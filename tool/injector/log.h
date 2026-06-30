#pragma once

#include <iostream>

#define Nullable(a) (a ? a : "<NULL>")
#define OutPrint(a) { std::wcout << "[" << __FUNCTION__ << "] " << a << std::endl; }

#ifdef _DEBUG 
	#define OutPrintVerbose(a) OutPrint(a)
#else
	#define OutPrintVerbose(a)
#endif