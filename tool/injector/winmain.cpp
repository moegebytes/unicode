#include "pch.h"
#include "winmain.h"
#include "core\injector.h"

int wmain(int argc, wchar_t* argv[]) {
	if (argc != 2 && argc != 3) {
		OutPrint("Usage:" << std::endl << "   " << argv[0] << " GameExecutable.exe [ShinkUnicode.dll]");
		return 1;
	}

	return Core::Injector::Run(argv[1], argc == 3 ? argv[2] : L"ShinkUnicode.dll");
}