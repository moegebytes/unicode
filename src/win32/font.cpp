#include "..\pch.h"
#include "font.h"
#include "..\util\font.h"
#include "..\util\text.h"

namespace Win32 {
	bool Font::JapaneseFaceName(HDC hDC, const LOGFONTA* pLogFont, CHAR (&szFaceName)[LF_FACESIZE]) {
		HFONT hFont = CreateFontIndirectA(pLogFont);
		if (!hFont) {
			return false;
		}

		HGDIOBJ hOldFont = SelectObject(hDC, hFont);

		CHAR szSelectedFace[LF_FACESIZE];
		bool bSelected = GetTextFaceA(hDC, LF_FACESIZE, szSelectedFace) > 0 && strcmp(szSelectedFace, pLogFont->lfFaceName) == 0;

		const std::wstring familyName = bSelected ? Util::GetFontFamilyName(hDC, LANGID_JAPANESE) : std::wstring();

		SelectObject(hDC, hOldFont);
		DeleteObject(hFont);

		if (familyName.empty()) {
			return false;
		}

		auto pszFaceName = Util::WideToNarrow(CP_SHIFT_JIS, familyName.c_str());
		return strncpy_s(szFaceName, LF_FACESIZE, pszFaceName.get(), _TRUNCATE) == 0;
	}
}