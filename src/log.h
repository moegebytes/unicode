#pragma once

#define Nullable(a) (a ? a : "<NULL>")
#define DbgPrint(a) { std::wstringstream ss; ss << "[" << __FUNCTION__ << "] " << a; OutputDebugString(ss.str().c_str()); }

#ifdef _DEBUG 
	#define DbgPrintVerbose(a) DbgPrint(a)
#else
	#define DbgPrintVerbose(a)
#endif