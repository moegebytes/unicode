#include "..\pch.h"
#include "hook.h"
#include "engine.h"
#include "window.h"
#include "render.h"
#include "..\hook\manager.h"
#include "..\util\env.h"

namespace FVP {
#if FVP_ENGINE_VER >= 10002
	bool Hook::Window::bIsResizing = false;
#endif

	void Hook::Install() {
		const auto hEngine = (uintptr_t)GetModuleHandle(NULL);
		HookManager::Install((bool(__thiscall*)(void*, const char*))(hEngine + FAVS::MoviePlayer::InitFilter),
			Hook::MoviePlayer::InitFilter);
#if FVP_ENGINE_VER >= 10002
		HookManager::Install(
			(HWND(__thiscall*)(void*))(hEngine + FAVS::Engine::InitWindow),
			Hook::Window::InitWindow);
		HookManager::Install(
			(LRESULT(__thiscall*)(void*, HWND, UINT, WPARAM, LPARAM))(hEngine + FAVS::Engine::PrimaryWindowProcA),
			Hook::Window::WndProc);
#endif
	}
}

namespace FVP {
	bool Hook::MoviePlayer::InitFilter(void* self, const char* fname) {
		if (!Util::IsWineEnvironment()) {
			return HookManager::Call(InitFilter, self, fname);
		}

		std::filesystem::path path(fname);
		if (path.filename().extension() == ".wmv" || path.filename().extension() == ".mpg") {
			path.replace_extension(".webm");
		}

		DbgPrintVerbose("Intercepted MoviePlayer::InitFilter with fname=" << fname);

		return HookManager::Call(InitFilter, self, path.string().c_str());
	}

#if FVP_ENGINE_VER >= 10002
	HWND Hook::Window::InitWindow(void* self) {
		DbgPrintVerbose("Intercepted Window::InitWindow");

		HWND hGameWnd = HookManager::Call(InitWindow, self);

		HWND hPrimaryWnd = FAVS::Field<HWND>(self, FAVS::Engine::Fields::PrimaryWindowHnd);
		if (!hPrimaryWnd) {
			return hGameWnd;
		}

		LONG lWndStyle = GetWindowLongW(hPrimaryWnd, GWL_STYLE);
		lWndStyle |= WS_THICKFRAME;
		SetWindowLongW(hPrimaryWnd, GWL_STYLE, lWndStyle);
		FAVS::Field<DWORD>(self, FAVS::Engine::Fields::DwStyle) = lWndStyle;

		RECT rc = { 
			0,
			0,
			FAVS::Field<int>(self, FAVS::Engine::Fields::GameW),
			FAVS::Field<int>(self, FAVS::Engine::Fields::GameH)
		};
		AdjustWindowRectEx(&rc, lWndStyle, FALSE, 0);

		SetWindowPos(
			hPrimaryWnd,
			NULL,
			0,
			0,
			rc.right - rc.left,
			rc.bottom - rc.top,
			SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED
		);

		FVP::Window::RestorePlacement(self, hPrimaryWnd, lWndStyle);

		return hGameWnd;
	}

	LRESULT Hook::Window::WndProc(void* self, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		DbgPrintVerbose("Intercepted Window::WndProc with uMsg=" << uMsg);

		double aspect = (double)FAVS::Field<int>(self, FAVS::Engine::Fields::GameW) / 
			(double)FAVS::Field<int>(self, FAVS::Engine::Fields::GameH);

		switch (uMsg) {
			case WM_GETMINMAXINFO: {
				MINMAXINFO* mmi = (MINMAXINFO*)lParam;

				DWORD dwStyle = FAVS::Field<DWORD>(self, FAVS::Engine::Fields::DwStyle);
				RECT rc = {
					0,
					0,
					FAVS::Field<int>(self, FAVS::Engine::Fields::GameW) * 2 / 3,
					FAVS::Field<int>(self, FAVS::Engine::Fields::GameH) * 2 / 3
				};
				AdjustWindowRectEx(&rc, dwStyle, FALSE, 0);

				mmi->ptMinTrackSize.x = rc.right - rc.left;
				mmi->ptMinTrackSize.y = rc.bottom - rc.top;

				rc = {
					0, 0,
					FAVS::Field<int>(self, FAVS::Engine::Fields::GameW),
					FAVS::Field<int>(self, FAVS::Engine::Fields::GameH)
				};
				AdjustWindowRectEx(&rc, dwStyle, FALSE, 0);

				mmi->ptMaxTrackSize.x = rc.right - rc.left;
				mmi->ptMaxTrackSize.y = rc.bottom - rc.top;
				return 0;
			}

			case WM_SIZING: {
				RECT frame = { 0, 0, 0, 0 };
				AdjustWindowRectEx(&frame, FAVS::Field<DWORD>(self, FAVS::Engine::Fields::DwStyle), FALSE, 0);

				int frameW = frame.right - frame.left;
				int frameH = frame.bottom - frame.top;

				RECT* client = (RECT*)lParam;
				int clientW = (client->right - client->left) - frameW;
				int clientH = (client->bottom - client->top) - frameH;

				switch (wParam) {
					case WMSZ_LEFT:
					case WMSZ_RIGHT:
						clientH = (int)(clientW / aspect);
						break;
					case WMSZ_TOP:
					case WMSZ_BOTTOM:
						clientW = (int)(clientH * aspect);
						break;
					default: {
						if (((double)clientW / (double)clientH) > aspect) {
							clientW = (int)(clientH * aspect);
						} else {
							clientH = (int)(clientW / aspect);
						}
						break;
					}
				}

				int totalW = clientW + frameW;
				int totalH = clientH + frameH;

				switch (wParam) {
					case WMSZ_LEFT:
					case WMSZ_TOPLEFT:
					case WMSZ_BOTTOMLEFT:
						client->left = client->right - totalW;
						break;
					default:
						client->right = client->left + totalW;
						break;
				}

				switch (wParam) {
					case WMSZ_TOP:
					case WMSZ_TOPLEFT:
					case WMSZ_TOPRIGHT:
						client->top = client->bottom - totalH;
						break;
					default:
						client->bottom = client->top + totalH;
						break;
				}

				return TRUE;
			}

			case WM_ENTERSIZEMOVE: {
				bIsResizing = true;
				break;
			}

			case WM_EXITSIZEMOVE: {
				bIsResizing = false;

				RECT rc;
				GetClientRect(hWnd, &rc);

				if (rc.right > 0 && rc.bottom > 0) {
					auto pRender = FAVS::Field<void*>(self, FAVS::Engine::Fields::Render);

					if (pRender && FAVS::Field<void*>(pRender, FAVS::Render::Fields::D3DDev)) {
						FVP::Window::UpdateScreen(self, rc.right, rc.bottom);
						FVP::Render::ResetDevice(pRender);
					}
				}

				FVP::Window::SavePlacement(self, hWnd);
				break;
			}

			case WM_SIZE: {
				if (wParam == SIZE_MINIMIZED) {
					break;
				}

				int clientW = LOWORD(lParam);
				int clientH = HIWORD(lParam);
				if (clientW <= 0 || clientH <= 0) {
					break;
				}

				void* pRender = FAVS::Field<void*>(self, FAVS::Engine::Fields::Render);
				if (!pRender || !FAVS::Field<void*>(pRender, FAVS::Render::Fields::D3DDev)) {
					break;
				}

				auto savedRenderFlag = FAVS::Field<DWORD>(pRender, FAVS::Render::Fields::SavedRenderFlag);
				auto currentRenderFlag = FAVS::Field<DWORD>(pRender, FAVS::Render::Fields::RenderFlag);

				if (bIsResizing) {
					HWND hGameWnd = FAVS::Field<HWND>(self, FAVS::Engine::Fields::GameWindowHnd);
					if (hGameWnd) {
						SetWindowPos(hGameWnd, NULL, 0, 0, clientW, clientH, SWP_NOZORDER);
					}
				} else if (currentRenderFlag == savedRenderFlag) {
					FVP::Window::UpdateScreen(self, clientW, clientH);
					FVP::Render::ResetDevice(pRender);
				}

				return HookManager::Call(WndProc, self, hWnd, uMsg, wParam, lParam);
			}

			case WM_SHOWWINDOW: {
				if (!wParam) {
					break;
				}

				void* pRender = FAVS::Field<void*>(self, FAVS::Engine::Fields::Render);
				if (!pRender || !FAVS::Field<void*>(pRender, FAVS::Render::Fields::D3DDev)) {
					break;
				}

				auto savedRenderFlag = FAVS::Field<DWORD>(pRender, FAVS::Render::Fields::SavedRenderFlag);
				auto currentRenderFlag = FAVS::Field<DWORD>(pRender, FAVS::Render::Fields::RenderFlag);

				if (currentRenderFlag == savedRenderFlag || currentRenderFlag == FAVS::Windowed) {
					PostMessage(hWnd, WM_RESTORE_PLACEMENT, 0, 0);
				}
				break;
			}

			case WM_RESTORE_PLACEMENT: {
				DWORD dwStyle = FAVS::Field<DWORD>(self, FAVS::Engine::Fields::DwStyle);
				if (!FVP::Window::RestorePlacement(self, hWnd, dwStyle)) {
					RECT rc = {
						0, 0,
						FAVS::Field<int>(self, FAVS::Engine::Fields::GameW) * 2 / 3,
						FAVS::Field<int>(self, FAVS::Engine::Fields::GameH) * 2 / 3
					};
					AdjustWindowRectEx(&rc, dwStyle, FALSE, 0);

					int cx = rc.right - rc.left;
					int cy = rc.bottom - rc.top;
					SetWindowPos(
						hWnd,
						NULL,
						(GetSystemMetrics(SM_CXSCREEN) - cx) / 2,
						(GetSystemMetrics(SM_CYSCREEN) - cy) / 2,
						cx,
						cy,
						SWP_NOZORDER
					);
				}

				RECT rc;
				GetClientRect(hWnd, &rc);
				if (rc.right > 0 && rc.bottom > 0) {
					void* pRender = FAVS::Field<void*>(self, FAVS::Engine::Fields::Render);
					if (pRender && FAVS::Field<void*>(pRender, FAVS::Render::Fields::D3DDev)) {
						FVP::Window::UpdateScreen(self, rc.right, rc.bottom);
						FVP::Render::ResetDevice(pRender);
					}
				}
				break;
			}
		}

		return HookManager::Call(WndProc, self, hWnd, uMsg, wParam, lParam);
	}
#endif
}