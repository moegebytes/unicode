#pragma once

#include "..\pch.h"

namespace Win32 {
	class Font {
	public:
		static bool JapaneseFaceName(HDC hDC, const LOGFONTA* pLogFont, CHAR (&szFaceName)[LF_FACESIZE]);
	};
}