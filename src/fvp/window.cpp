#include "..\pch.h"
#include "window.h"
#include "engine.h"
#include "..\util\describe.h"

namespace FVP {
#if FVP_GAME_ID >= HOSHINOMEMORIA
	inline std::filesystem::path Window::GetPlacementPath(void* instance) {
		const char* base = FAVS::Field<const char*>(instance, FAVS::Engine::Fields::LocalDataPath);
		return std::filesystem::path(base) / "window.bin";
	}

	SIZE Window::GetDefaultClientSize(int gameW, int gameH) {
		double scaleW = (double)GetSystemMetrics(SM_CXSCREEN) * 2.0 / 3.0 / (double)gameW;
		double scaleH = (double)GetSystemMetrics(SM_CYSCREEN) * 2.0 / 3.0 / (double)gameH;

		auto scale = (scaleW < scaleH) ? scaleW : scaleH;
		if (scale >= 1.0) {
			return { gameW, gameH };
		}

		return { (LONG)(gameW * scale), (LONG)(gameH * scale) };
	}

	void Window::UpdateScreen(void* self, int w, int h) {
		FAVS::Field<DWORD>(self, FAVS::Engine::Fields::ScreenW) = w;
		FAVS::Field<DWORD>(self, FAVS::Engine::Fields::ScreenH) = h;

		HWND hGameWnd = FAVS::Field<HWND>(self, FAVS::Engine::Fields::GameWindowHnd);
		if (hGameWnd) {
			SetWindowPos(hGameWnd, NULL, 0, 0, w, h, SWP_NOZORDER);
		}
	}

	void Window::SavePlacement(void* instance, HWND hWnd) {
		RECT rc;
		if (!GetClientRect(hWnd, &rc)) {
			DbgPrintVerbose("Unable to retrieve window dimensions; code=" << GetLastError());
			return;
		}

		PlacementData data = {
			rc.right,
			rc.bottom
		};

		auto path = GetPlacementPath(instance).string();
		HANDLE hFile = CreateFileA(path.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) {
			DbgPrint("Failed to persist window state; code=" << GetLastError());
			return;
		}

		DWORD nBytesWritten;
		WriteFile(hFile, &data, sizeof(data), &nBytesWritten, NULL);
		CloseHandle(hFile);

		if (nBytesWritten != sizeof(data)) {
			DbgPrint("Failed to persist window state; code=" << GetLastError());
		}
	}

	bool Window::RestorePlacement(void* instance, HWND hWnd, DWORD dwStyle) {
		auto path = GetPlacementPath(instance).string();
		HANDLE hFile = CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE) {
			DbgPrint("Failed to restore window state; code=" << GetLastError());
			return false;
		}

		PlacementData data;

		DWORD nBytesRead;
		BOOL bResult = ReadFile(hFile, &data, sizeof(data), &nBytesRead, NULL);
		CloseHandle(hFile);

		if (!bResult || nBytesRead != sizeof(data) || data.width <= 0 || data.height <= 0) {
			DbgPrint("Failed to restore window state; code=" << GetLastError());
			return false;
		}

		RECT rc = { 0, 0, data.width, data.height };
		AdjustWindowRectEx(&rc, dwStyle, FALSE, 0);

		SetWindowPos(
			hWnd,
			NULL,
			0, 0,
			rc.right - rc.left,
			rc.bottom - rc.top,
			SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED
		);

		DbgPrintVerbose("Restored window state=" << Util::Describe(data));
		return true;
	}
#endif
}