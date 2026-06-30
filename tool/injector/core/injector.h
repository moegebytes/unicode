#pragma once

#include "..\pch.h"
#include <detours.h>

namespace Core {
	class Injector {
	public:
		static int Run(LPCWSTR lpExecutablePath, LPCWSTR lpDllPath);

	private:
		class Callback {
		public:
			struct ImportContext {
				std::vector<std::string> vDllPaths;
				size_t iDllPath;
			};

			static BOOL CALLBACK FindOrdinal1Callback(PVOID pContext, ULONG nOrdinal, LPCSTR, PVOID);
			static BOOL CALLBACK CollectImportCallback(PVOID pContext, LPCSTR lpFile, LPCSTR* lpOutFile);
			static BOOL CALLBACK AddImportCallback(PVOID pContext, LPCSTR lpFile, LPCSTR* lpOutFile);
		};
	};
}