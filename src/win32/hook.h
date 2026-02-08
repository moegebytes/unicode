#pragma once

#include "..\pch.h"
#include <mmiscapi.h>

namespace Win32 {
	class Hook {
	public:
		static void Install();
	private:
		static UINT WINAPI GetOEMCP();
		static UINT WINAPI GetACP();
		static BOOL WINAPI GetCPInfo(UINT CodePage, LPCPINFO lpCPInfo);
		static int WINAPI lstrcmpiA(LPCSTR lpString1, LPCSTR lpString2);
		static HANDLE WINAPI CreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
			DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile
		);
		static int WINAPI MultiByteToWideChar(UINT CodePage, DWORD dwFlags, LPCCH lpMultiByteStr, int cbMultiByte, LPWSTR lpWideCharStr, int ccWideChar);
		static int WINAPI WideCharToMultiByte(UINT CodePage, DWORD dwFlags, LPCWCH lpWideCharStr, int cchWideChar, LPSTR lpMultiByteStr, int cbMultiByte,
			LPCCH lpDefaultChar, LPBOOL lpUsedDefaultChar
		);

		static int WINAPI MessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);
		static ATOM WINAPI RegisterClassExA(const WNDCLASSEXA* lpWndClassA);
		static HWND WINAPI CreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight,
			HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam
		);
		static LRESULT WINAPI DefWindowProcA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		static LRESULT WINAPI DispatchMessageA(MSG* lpMsg);

		static HMMIO WINAPI mmioOpenA(LPSTR pszFileName, LPMMIOINFO pmmioinfo, DWORD fdwOpen);

		static HFONT WINAPI CreateFontA(int cHeight, int cWidth, int cEscapement, int cOrientation, int cWeight, DWORD bItalic, DWORD bUnderline,
			DWORD bStrikeOut, DWORD iCharSet, DWORD iOutPrecision, DWORD iClipPrecision, DWORD iQuality, DWORD iPitchAndFamily, LPCSTR pszFaceName
		);
		static int WINAPI EnumFontFamiliesExA(HDC hdc, LPLOGFONTA lpLogfont, FONTENUMPROCA lpProc, LPARAM lParam, DWORD dwFlags);

		class Callback {
		public:
			struct EnumFontParam {
				FONTENUMPROCA lpProc;
				LPARAM lParam;
			};

			static int CALLBACK EnumFontFamExProcA(const LOGFONTA* lpfnta, const TEXTMETRICA* lpntme, DWORD FontType, LPARAM lParam);
		};
	};
}