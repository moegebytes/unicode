#include "pch.h"
#include "dllmain.h"
#include "win32\hook.h"
#include "fvp\font.h"
#include "fvp\hook.h"

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD fdwReason, LPVOID lpReserved) {
	if (hInstance) {
		DisableThreadLibraryCalls(hInstance);
	}

	switch (fdwReason) {
		case DLL_PROCESS_ATTACH:
			Win32::Hook::Install();
			FVP::Font::Load();
			FVP::Hook::Install();
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}

	return TRUE;
}

extern "C" __declspec(dllexport) VOID ShinkUnicode() {
	return;
}