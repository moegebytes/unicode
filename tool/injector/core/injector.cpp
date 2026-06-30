
#include "..\pch.h"
#include "injector.h"
#include "..\util\text.h"

namespace Core {
	int Injector::Run(LPCWSTR lpExecutablePath, LPCWSTR lpDllPath) {
		OutPrintVerbose("Running with lpExecutablePath=" << lpExecutablePath << ",lpShinkuDllPath=" << lpDllPath);

		DWORD dwAttributes = GetFileAttributesW(lpExecutablePath);
		if (dwAttributes == INVALID_FILE_ATTRIBUTES || (dwAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
			OutPrint("Invalid executable path provided; attribs=" << dwAttributes << ",code=" << GetLastError());
			return 1;
		}

		dwAttributes = GetFileAttributesW(lpDllPath);
		if (dwAttributes == INVALID_FILE_ATTRIBUTES || (dwAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
			OutPrint("Invalid DLL path provided; attribs=" << dwAttributes << ",code=" << GetLastError());
			return 1;
		}

		HMODULE hDll = LoadLibraryExW(lpDllPath, NULL, DONT_RESOLVE_DLL_REFERENCES);
		if (!hDll) {
			OutPrint("Could not load DLL for export verification; code=" << GetLastError());
			return 1;
		}

		BOOL bHasOrdinal1 = FALSE;
		DetourEnumerateExports(hDll, &bHasOrdinal1, Callback::FindOrdinal1Callback);
		FreeLibrary(hDll);

		if (!bHasOrdinal1) {
			OutPrint("Selected DLL does not export function with ordinal #1");
			return 1;
		}

		HANDLE hFile = CreateFileW(
			lpExecutablePath,
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (hFile == INVALID_HANDLE_VALUE) {
			OutPrint("Could not open executable file for read/write; code=" << GetLastError());
			return 1;
		}

		auto pBinary = DetourBinaryOpen(hFile);
		if (!pBinary) {
			OutPrint("Could not parse executable file; code=" << GetLastError());

			CloseHandle(hFile);
			return 1;
		}

		Callback::ImportContext context = {
			{},
			0
		};

		if (!DetourBinaryEditImports(pBinary, &context, Callback::CollectImportCallback, NULL, NULL, NULL)) {
			OutPrint("Unable to enumerate byway imports; code=" << GetLastError());

			DetourBinaryClose(pBinary);
			CloseHandle(hFile);
			return 1;
		}

		auto lpNarrowDllName = Util::WideToNarrow(std::filesystem::path(lpDllPath).filename().c_str());
		BOOL bFound = FALSE;

		for (auto &lpExistingDllPath: context.vDllPaths) {
			if (lstrcmpiA(lpExistingDllPath.c_str(), lpNarrowDllName.get()) == 0) {
				bFound = TRUE;
				break;
			}
		}

		if (bFound) {
			DetourBinaryClose(pBinary);
			CloseHandle(hFile);

			OutPrint("Binary is already patched");
			return 0;
		}

		context.vDllPaths.push_back(lpNarrowDllName.get());
		DetourBinaryResetImports(pBinary);

		if (!DetourBinaryEditImports(pBinary, &context, Callback::AddImportCallback, NULL, NULL, NULL)) {
			OutPrint("Unable to inject byway imports; code=" << GetLastError());

			DetourBinaryClose(pBinary);
			CloseHandle(hFile);
			return 1;
		}

		if (!DetourBinaryWrite(pBinary, hFile)) {
			OutPrint("Unable to write patched executable; code=" << GetLastError());

			DetourBinaryClose(pBinary);
			CloseHandle(hFile);
			return 1;
		}

		DetourBinaryClose(pBinary);
		CloseHandle(hFile);

		OutPrint("Binary has been patched successfully");
		return 0;
	}
}

namespace Core {
	BOOL CALLBACK Injector::Callback::FindOrdinal1Callback(PVOID pContext, ULONG nOrdinal, LPCSTR, PVOID) {
		if (nOrdinal == 1) {
			*(BOOL*)pContext = TRUE;
		}

		return TRUE;
	}

	BOOL CALLBACK Injector::Callback::CollectImportCallback(PVOID pContext, LPCSTR lpFile, LPCSTR* lpOutFile) {
		ImportContext* importContext = (ImportContext*)pContext;
		*lpOutFile = lpFile;

		if (lpFile) {
			OutPrintVerbose("Found existing byway import; lpFile=" << lpFile);

			importContext->vDllPaths.push_back(lpFile);
		}

		return TRUE;
	}

	BOOL CALLBACK Injector::Callback::AddImportCallback(PVOID pContext, LPCSTR lpFile, LPCSTR* lpOutFile) {
		ImportContext* importContext = (ImportContext*)pContext;
		*lpOutFile = lpFile;

		if (!lpFile && importContext->iDllPath < importContext->vDllPaths.size()) {
			const auto &lpDllPath = importContext->vDllPaths[importContext->iDllPath++];
			OutPrintVerbose("Adding new byway import; lpDllPath=" << lpDllPath.c_str());

			*lpOutFile = lpDllPath.c_str();
		}

		return TRUE;
	}
}