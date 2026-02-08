#include "..\pch.h"
#include "hook.h"
#include "..\hook\manager.h"
#include "..\util\text.h"
#include "..\util\locale.h"

namespace Win32 {
	void Hook::Install() {
		const auto hKernel32 = GetModuleHandle(L"kernel32.dll");
		HookManager::Install((UINT(WINAPI *) ())GetProcAddress(hKernel32, "GetOEMCP"), GetOEMCP);
		HookManager::Install((UINT(WINAPI *) ())GetProcAddress(hKernel32, "GetACP"), GetACP);
		HookManager::Install((BOOL(WINAPI *) (UINT, LPCPINFO))GetProcAddress(hKernel32, "GetCPInfo"), GetCPInfo);
		HookManager::Install((int(WINAPI *) (LPCSTR, LPCSTR))GetProcAddress(hKernel32, "lstrcmpiA"), lstrcmpiA);
		HookManager::Install(
			(HANDLE(WINAPI *) (LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE))GetProcAddress(hKernel32, "CreateFileA"),
			CreateFileA);
		HookManager::Install((int(WINAPI *) (UINT, DWORD, LPCCH, int, LPWSTR, int))GetProcAddress(hKernel32, "MultiByteToWideChar"),
			MultiByteToWideChar);
		HookManager::Install((int(WINAPI *) (UINT, DWORD, LPCWCH, int, LPSTR, int, LPCCH, LPBOOL))GetProcAddress(hKernel32, "WideCharToMultiByte"),
			WideCharToMultiByte);

		const auto hUser32 = GetModuleHandle(L"user32.dll");
		HookManager::Install((int(WINAPI *) (HWND, LPCSTR, LPCSTR, UINT))GetProcAddress(hUser32, "MessageBoxA"), MessageBoxA);
		HookManager::Install((ATOM(WINAPI *) (const WNDCLASSEXA*))GetProcAddress(hUser32, "RegisterClassExA"), RegisterClassExA);
		HookManager::Install(
			(HWND(WINAPI *) (DWORD, LPCSTR, LPCSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, LPVOID))GetProcAddress(hUser32, "CreateWindowExA"),
			CreateWindowExA);
		HookManager::Install((LRESULT(WINAPI *) (HWND, UINT, WPARAM, LPARAM))GetProcAddress(hUser32, "DefWindowProcA"), DefWindowProcA);
		HookManager::Install((LRESULT(WINAPI *) (MSG*))GetProcAddress(hUser32, "DispatchMessageA"), DispatchMessageA);

		const auto hWinmm = GetModuleHandle(L"winmm.dll");
		HookManager::Install((HMMIO(WINAPI *) (LPSTR, LPMMIOINFO, DWORD))GetProcAddress(hWinmm, "mmioOpenA"), mmioOpenA);

		const auto hGdi32 = GetModuleHandle(L"gdi32.dll");
		HookManager::Install(
			(HFONT(WINAPI *) (int, int, int, int, int, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, LPCSTR))GetProcAddress(hGdi32, "CreateFontA"),
			CreateFontA);
		HookManager::Install(
			(int(CALLBACK *) (HDC, LPLOGFONTA, FONTENUMPROCA, LPARAM, DWORD))GetProcAddress(hGdi32, "EnumFontFamiliesExA"),
			EnumFontFamiliesExA);
	}

	UINT WINAPI Hook::GetOEMCP() {
		DbgPrintVerbose("Intercepted GetOEMCP");
		return (UINT)932;
	}

	UINT WINAPI Hook::GetACP() {
		DbgPrintVerbose("Intercepted GetACP");
		return (UINT)932;
	}

	BOOL WINAPI Hook::GetCPInfo(UINT CodePage, LPCPINFO lpCPInfo) {
		if (!Util::IsOSDependantCodePage(CodePage)) {
			return HookManager::Call(GetCPInfo, CodePage, lpCPInfo);
		}

		DbgPrintVerbose("Intercepted GetCPInfo with CodePage=" << CodePage);
		return HookManager::Call(GetCPInfo, (UINT)932, lpCPInfo);
	}

	int WINAPI Hook::lstrcmpiA(LPCSTR lpString1, LPCSTR lpString2) {
		DbgPrintVerbose("Intercepted lstrcmpiA with lpString1=" << lpString1 << ",lpString2=" << lpString2);
		return (CompareStringA(LCID_JAPANESE, 1, lpString1, -1, lpString2, -1)) - 2;
	}

	HANDLE WINAPI Hook::CreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile
	) {
		DbgPrintVerbose("Intercepted CreateFileA with lpFileName=" << lpFileName);

		return CreateFileW(
			Util::NarrowToWide(932, lpFileName).get(), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition,
			dwFlagsAndAttributes, hTemplateFile);
	}

	int WINAPI Hook::MultiByteToWideChar(UINT CodePage, DWORD dwFlags, LPCCH lpMultiByteStr, int cbMultiByte, LPWSTR lpWideCharStr, int ccWideChar) {
		if (!Util::IsOSDependantCodePage(CodePage)) {
			return HookManager::Call(MultiByteToWideChar, CodePage, dwFlags, lpMultiByteStr, cbMultiByte, lpWideCharStr, ccWideChar);
		}

		DbgPrintVerbose("Intercepted MultiByteToWideChar with CodePage=" << CodePage << ",lpMultiByteStr=" << lpMultiByteStr);
		return HookManager::Call(MultiByteToWideChar, (UINT)932, dwFlags, lpMultiByteStr, cbMultiByte, lpWideCharStr, ccWideChar);
	}

	int WINAPI Hook::WideCharToMultiByte(UINT CodePage, DWORD dwFlags, LPCWCH lpWideCharStr, int cchWideChar, LPSTR lpMultiByteStr, int cbMultiByte,
		LPCCH lpDefaultChar, LPBOOL lpUsedDefaultChar
	) {
		if (!Util::IsOSDependantCodePage(CodePage)) {
			return HookManager::Call(WideCharToMultiByte, CodePage, dwFlags, lpWideCharStr, cchWideChar, lpMultiByteStr, cbMultiByte, 
				lpDefaultChar, lpUsedDefaultChar);
		}

		DbgPrintVerbose("Intercepted WideCharToMultiByte with CodePage=" << CodePage << ",lpWideCharStr=" << lpWideCharStr);
		return HookManager::Call(WideCharToMultiByte, (UINT)932, dwFlags, lpWideCharStr, cchWideChar, lpMultiByteStr, cbMultiByte, 
			lpDefaultChar, lpUsedDefaultChar);
	}

	int WINAPI Hook::MessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) {
		DbgPrintVerbose("Intercepted MessageBoxA with lpText=" << Nullable(lpText) << ",lpCaption=" << Nullable(lpCaption));

		auto lpWideText = lpText ? Util::NarrowToWide(932, lpText) : NULL;
		auto lpWideCaption = lpCaption ? Util::NarrowToWide(932, lpCaption) : NULL;

		return MessageBoxW(hWnd, lpWideText.get(), lpWideCaption.get(), uType);
	}

	ATOM WINAPI Hook::RegisterClassExA(const WNDCLASSEXA* lpWndClassA) {
		// WNDCLASSEX.lpszClassName is defined as being either null-terminated string or ATOM, but passing
		// value of ATOM type to RegisterClassEx is invalid scenario, so we do not check for it here, like
		// we do in CreateWindowsEx below.
		if (strcmp(lpWndClassA->lpszClassName, "HS_MAIN_WINDOW_CLASS00")) {
			return HookManager::Call(RegisterClassExA, lpWndClassA);
		}

		DbgPrintVerbose("Intercepted RegisterClassExA with lpClassName=" << lpWndClassA->lpszClassName);

		// Ensure we bind unique_ptr to current scope, so that its usage in initialization of lpWndClassW is valid
		// and does not cause deallocation before call to RegisterClassExW.
		auto lpszWideMenuName = lpWndClassA->lpszMenuName ? Util::NarrowToWide(932, lpWndClassA->lpszMenuName) : NULL;
		auto lpszWideClassName = Util::NarrowToWide(932, lpWndClassA->lpszClassName);

		// Passing lpfnWndProc as-is is fine, as FVP callbacks do not process any messages which might contain
		// strings as parameters.
		WNDCLASSEXW lpWndClassW = {
			sizeof(WNDCLASSEXW),
			lpWndClassA->style,
			lpWndClassA->lpfnWndProc,
			lpWndClassA->cbClsExtra,
			lpWndClassA->cbWndExtra,
			lpWndClassA->hInstance,
			lpWndClassA->hIcon,
			lpWndClassA->hCursor,
			lpWndClassA->hbrBackground,
			lpszWideMenuName.get(),
			lpszWideClassName.get(),
			lpWndClassA->hIconSm
		};

		if (lpWndClassA->lpszMenuName) assert(lpszWideMenuName.get() != nullptr);
		assert(lpszWideClassName.get() != nullptr);

		return RegisterClassExW(&lpWndClassW);
	}

	HWND WINAPI Hook::CreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight,
		HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam
	) {
		if (IS_ATOM(lpClassName) || strcmp(lpClassName, "HS_MAIN_WINDOW_CLASS00")) {
			return HookManager::Call(CreateWindowExA, dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, 
				hInstance, lpParam);
		}

		DbgPrintVerbose("Intercepted CreateWindowExA with lpClassName=" << lpClassName << ",lpWindowName=" << Nullable(lpWindowName));

		auto lpWideClassName = Util::NarrowToWide(932, lpClassName);
		auto lpWideWindowName = lpWindowName ? Util::NarrowToWide(932, lpWindowName) : NULL;

		return CreateWindowExW(dwExStyle, lpWideClassName.get(), lpWideWindowName.get(),
			dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
	}

	LRESULT WINAPI Hook::DefWindowProcA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		if (!IsWindowUnicode(hWnd)) {
			return HookManager::Call(DefWindowProcA, hWnd, uMsg, wParam, lParam);
		}

		DbgPrintVerbose("Intercepted DefWindowProcA with uMsg=" << uMsg);
		return DefWindowProcW(hWnd, uMsg, wParam, lParam);
	}

	LRESULT WINAPI Hook::DispatchMessageA(MSG* lpMsg) {
		if (!IsWindowUnicode(lpMsg->hwnd)) {
			return HookManager::Call(DispatchMessageA, lpMsg);
		}
	
		DbgPrintVerbose("Intercepted DispatchMessageA with uMsg=" << lpMsg->message);
		return DispatchMessageW(lpMsg);
	}

	HMMIO WINAPI Hook::mmioOpenA(LPSTR pszFileName, LPMMIOINFO pmmioinfo, DWORD fdwOpen) {
		if (!pszFileName) {
			return HookManager::Call(mmioOpenA, pszFileName, pmmioinfo, fdwOpen);
		}

		DbgPrintVerbose("Intercepted mmioOpenA with pszFileName=" << pszFileName);
		return mmioOpenW(Util::NarrowToWide(932, pszFileName).get(), pmmioinfo, fdwOpen);
	}

	HFONT WINAPI Hook::CreateFontA(int cHeight, int cWidth, int cEscapement, int cOrientation, int cWeight, DWORD bItalic, DWORD bUnderline,
		DWORD bStrikeOut, DWORD iCharSet, DWORD iOutPrecision, DWORD iClipPrecision, DWORD iQuality, DWORD iPitchAndFamily, LPCSTR pszFaceName
	) {
		DbgPrintVerbose("Intercepted CreateFontA with iCharSet=" << iCharSet << ",pszFaceName=" << Nullable(pszFaceName));

		auto pszWideFaceName = pszFaceName ? Util::NarrowToWide(932, pszFaceName) : NULL;

		return CreateFontW(cHeight, 0, cEscapement, cOrientation, cWeight, bItalic, bUnderline, bStrikeOut, SHIFTJIS_CHARSET, iOutPrecision, 
			iClipPrecision, iQuality, iPitchAndFamily, pszWideFaceName.get());
	}

	int WINAPI Hook::EnumFontFamiliesExA(HDC hdc, LPLOGFONTA lpLogfont, FONTENUMPROCA lpProc, LPARAM lParam, DWORD dwFlags) {
		DbgPrintVerbose("Intercepted EnumFontFamiliesExA with lfCharSet=" << lpLogfont->lfCharSet);

		Callback::EnumFontParam sParam = {
			lpProc,
			lParam
		};

		lpLogfont->lfCharSet = ANSI_CHARSET;
		return HookManager::Call(EnumFontFamiliesExA, hdc, lpLogfont, Callback::EnumFontFamExProcA, (LPARAM)&sParam, dwFlags);
	}
}

namespace Win32 {
	int CALLBACK Hook::Callback::EnumFontFamExProcA(const LOGFONTA* lpfnta, const TEXTMETRICA* lpntme, DWORD FontType, LPARAM lParam) {
		ENUMLOGFONTEXA* lpelfe = (ENUMLOGFONTEXA*)lpfnta;

		if (lpelfe->elfLogFont.lfFaceName[0] != '@') {
			DbgPrintVerbose("Enumerated font with lfFaceName=" << lpelfe->elfLogFont.lfFaceName << ",elfFullName=" <<
				(CHAR*)lpelfe->elfFullName << ",elfScript=" << (CHAR*)lpelfe->elfScript);
		}

		memset(lpelfe->elfScript, 0, LF_FACESIZE); // It's fine if it gets optimized away, string below is zero-terminated
		static const unsigned char scriptJapanese[] = { 0x93, 0xFA, 0x96, 0x7B, 0x8C, 0xEA, 0x00 }; // 日本語\0
		memcpy(lpelfe->elfScript, scriptJapanese, 7);

		EnumFontParam* sParam = (EnumFontParam*)lParam;
		return sParam->lpProc((LOGFONTA*)lpelfe, lpntme, FontType, sParam->lParam);
	}
}