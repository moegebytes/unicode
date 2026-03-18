#include "..\pch.h"
#include "hook.h"
#include "engine.h"
#include "window.h"
#include "render.h"
#include "..\hook\manager.h"
#include "..\util\env.h"

namespace FVP {
	void Hook::Install() {
		const auto hEngine = (uintptr_t)GetModuleHandle(NULL);
		HookManager::Install((bool(__thiscall*)(void*, const char*))(hEngine + FAVS::MoviePlayer::InitFilter),
			Hook::MoviePlayer::InitFilter);
#if FVP_GAME_ID >= HOSHINOMEMORIA
		HookManager::Install(
			(HWND(__thiscall*)(void*))(hEngine + FAVS::Engine::InitWindow),
			Hook::Engine::InitWindow);
		HookManager::Install(
			(LRESULT(__thiscall*)(void*, HWND, UINT, WPARAM, LPARAM))(hEngine + FAVS::Engine::PrimaryWindowProcA),
			Hook::Engine::PrimaryWindowProcA);
#endif
	}

	bool Hook::MoviePlayer::InitFilter(void* self, const char* fname) {
		if (!Util::IsWineEnvironment()) {
			return HookManager::Call(InitFilter, self, fname);
		}

		std::filesystem::path path(fname);
		if (path.filename().extension() == ".wmv" || path.filename().extension() == ".mpg") {
			// Wine needs additional setup to render Windows Media Video files (`winetricks wmp9`),
			// but generally supports VP8 out of the box on most distros. Idea stolen from
			// NekoNyan's IroSeka patch.
			path.replace_extension(".webm");
		}

		DbgPrintVerbose("Intercepted MoviePlayer::InitFilter with fname=" << fname);

		return HookManager::Call(InitFilter, self, path.string().c_str());
	}

#if FVP_GAME_ID >= HOSHINOMEMORIA
	HWND Hook::Engine::InitWindow(void* self) {
		DbgPrintVerbose("Intercepted Engine::InitWindow");

		HWND hGameWnd = HookManager::Call(InitWindow, self);

		HWND hPrimaryWnd = FAVS::Field<HWND>(self, FAVS::Engine::Fields::PrimaryWindowHnd);
		if (!hPrimaryWnd) {
			return hGameWnd;
		}

		LONG lWndStyle = GetWindowLongW(hPrimaryWnd, GWL_STYLE);
		lWndStyle |= WS_THICKFRAME; // Enables resizing frame
		SetWindowLongW(hPrimaryWnd, GWL_STYLE, lWndStyle);

		// Engine stores window style in order to restore it when toggling between fullscreen and windowed mode
		FAVS::Field<DWORD>(self, FAVS::Engine::Fields::DwStyle) = lWndStyle;

		RECT rc = { 
			0, 0,
			FAVS::Field<int>(self, FAVS::Engine::Fields::GameW),
			FAVS::Field<int>(self, FAVS::Engine::Fields::GameH)
		};
		AdjustWindowRectEx(&rc, lWndStyle, FALSE, 0);

		SetWindowPos(
			hPrimaryWnd,
			NULL,
			0, 0,
			rc.right - rc.left,
			rc.bottom - rc.top,
			SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED
		);

		return hGameWnd;
	}

	LRESULT Hook::Engine::PrimaryWindowProcA(void* self, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		DbgPrintVerbose("Intercepted Engine::PrimaryWindowProcA with uMsg=" << uMsg);

		double aspect = static_cast<double>(FAVS::Field<int>(self, FAVS::Engine::Fields::GameW)) /
			static_cast<double>(FAVS::Field<int>(self, FAVS::Engine::Fields::GameH));

		switch (uMsg) {
			case WM_GETMINMAXINFO: {
				MINMAXINFO* mmi = (MINMAXINFO*)lParam;

				SIZE defSize = FVP::Window::GetMinimumWindowSize(
					FAVS::Field<int>(self, FAVS::Engine::Fields::GameW),
					FAVS::Field<int>(self, FAVS::Engine::Fields::GameH)
				);
				RECT rc = { 0, 0, defSize.cx, defSize.cy };
				AdjustWindowRectEx(&rc, FAVS::Field<DWORD>(self, FAVS::Engine::Fields::DwStyle), FALSE, 0);

				mmi->ptMinTrackSize.x = rc.right - rc.left;
				mmi->ptMinTrackSize.y = rc.bottom - rc.top;

				// Maximum window size is clamped to native game resolution to prevent upscaling
				rc = {
					0, 0,
					FAVS::Field<int>(self, FAVS::Engine::Fields::GameW),
					FAVS::Field<int>(self, FAVS::Engine::Fields::GameH)
				};
				AdjustWindowRectEx(&rc, FAVS::Field<DWORD>(self, FAVS::Engine::Fields::DwStyle), FALSE, 0);

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
						clientH = static_cast<int>(clientW / aspect);
						break;
					case WMSZ_TOP:
					case WMSZ_BOTTOM:
						clientW = static_cast<int>(clientH * aspect);
						break;
					default: {
						if (static_cast<double>(clientW) / static_cast<double>(clientH) > aspect) {
							clientW = static_cast<int>(clientH * aspect);
						} else {
							clientH = static_cast<int>(clientW / aspect);
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

			case WM_EXITSIZEMOVE: {
				RECT rc;
				GetClientRect(hWnd, &rc);

				if (rc.right > 0 && rc.bottom > 0) {
					auto pRender = FAVS::Field<void*>(self, FAVS::Engine::Fields::Render);
					if (pRender && FAVS::Field<void*>(pRender, FAVS::Render::Fields::D3DDev)) {
						FVP::Window::UpdateScreen(self, rc.right, rc.bottom);
						FVP::Render::ResetDevice(pRender);
					}
				}

				FVP::Window::SaveDimensions(self, hWnd);
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

				// Engine performs downscaling on reset which is rather expensive operation, causing visibile staggering during resize.
				// To avoid it, we only update game window size here, which causes internal upscaling take over.
				// This causes bluriness, but// only during actual resize, as once user let's go, WM_EXITSIZEMOVE fires and performs
				// full reset.
				// If we performed no action here, the outer window would resize but game window would remain at fixed size, causing
				// rest to be filled with black. There's no perfect solution here, sadly.
				HWND hGameWnd = FAVS::Field<HWND>(self, FAVS::Engine::Fields::GameWindowHnd);
				if (hGameWnd) {
					SetWindowPos(hGameWnd, NULL, 0, 0, clientW, clientH, SWP_NOZORDER);
				}

				return HookManager::Call(PrimaryWindowProcA, self, hWnd, uMsg, wParam, lParam);
			}

			case WM_SHOWWINDOW: {
				if (!wParam) {
					break;
				}

				auto pRender = FAVS::Field<void*>(self, FAVS::Engine::Fields::Render);
				if (!pRender || !FAVS::Field<void*>(pRender, FAVS::Render::Fields::D3DDev)) {
					break;
				}

				if (FAVS::Field<DWORD>(pRender, FAVS::Render::Fields::RenderFlag) == FAVS::Windowed) {
					PostMessage(hWnd, WM_RESTORE_SIZE, 0, 0);
				}
				break;
			}

			case WM_RESTORE_SIZE: {
				if (!FVP::Window::RestoreDimensions(self, hWnd, FAVS::Field<DWORD>(self, FAVS::Engine::Fields::DwStyle))) {
					SIZE defSize = FVP::Window::GetOptimalWindowSize(
						FAVS::Field<int>(self, FAVS::Engine::Fields::GameW),
						FAVS::Field<int>(self, FAVS::Engine::Fields::GameH)
					);
					RECT rc = { 0, 0, defSize.cx, defSize.cy };
					AdjustWindowRectEx(&rc, FAVS::Field<DWORD>(self, FAVS::Engine::Fields::DwStyle), FALSE, 0);

					// Ensures window is displayed at the center of screen
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
					auto pRender = FAVS::Field<void*>(self, FAVS::Engine::Fields::Render);
					if (pRender && FAVS::Field<void*>(pRender, FAVS::Render::Fields::D3DDev)) {
						FVP::Window::UpdateScreen(self, rc.right, rc.bottom);
						FVP::Render::ResetDevice(pRender);
					}
				}
				break;
			}
		}

		return HookManager::Call(PrimaryWindowProcA, self, hWnd, uMsg, wParam, lParam);
	}
#endif
}