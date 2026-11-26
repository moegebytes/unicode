#include "..\pch.h"
#include "font.h"

namespace {
	struct BIGWORD {
		BYTE high;
		BYTE low;

		operator WORD() const {
			return static_cast<WORD>((high << 8) | low);
		}
	};

	struct NameHeader {
		BIGWORD format;
		BIGWORD count;
		BIGWORD stringOffset;
	};

	struct NameRecord {
		BIGWORD platformId;
		BIGWORD encodingId;
		BIGWORD languageId;
		BIGWORD nameId;
		BIGWORD length;
		BIGWORD offset;
	};

	static_assert(sizeof(NameHeader) == 6 && alignof(NameHeader) == 1);
	static_assert(sizeof(NameRecord) == 12 && alignof(NameRecord) == 1);

	class NameTable {
	public:
		explicit NameTable(std::span<const BYTE> data) : m_data(data) {}

		std::span<const NameRecord> Records() const {
			if (m_data.size() < sizeof(NameHeader)) {
				return {};
			}

			const size_t maxCount = (m_data.size() - sizeof(NameHeader)) / sizeof(NameRecord);
			return {reinterpret_cast<const NameRecord*>(&Header() + 1), min(static_cast<size_t>(Header().count), maxCount)};
		}

		std::wstring GetString(const NameRecord& record) const {
			const size_t start = Header().stringOffset + record.offset;
			if (start + record.length > m_data.size()) {
				return {};
			}

			const auto pChars = reinterpret_cast<const BIGWORD*>(m_data.data() + start);
			return std::wstring(pChars, pChars + record.length / sizeof(BIGWORD));
		}

	private:
		const NameHeader& Header() const {
			return *reinterpret_cast<const NameHeader*>(m_data.data());
		}

		std::span<const BYTE> m_data;
	};

	bool IsFamilyName(const NameRecord& record) {
		return record.platformId == 3 /* PlatformWindows */ && record.nameId == 1 /* FamilyNameId */
			&& (record.encodingId == 1 /* UnicodeBmp */ || record.encodingId == 10 /* UnicodeFull */);
	}

	LANGID GetLanguage(const NameRecord& record) {
		return record.languageId;
	}
}

namespace Util {
	std::wstring GetFontFamilyName(HDC hDC, LANGID langId) {
		DWORD dwSize = GetFontData(hDC, 0x656D616E, 0, NULL, 0);
		if (dwSize == GDI_ERROR) {
			return {};
		}

		auto pData = std::make_unique<BYTE[]>(dwSize);
		if (GetFontData(hDC, 0x656D616E, 0, pData.get(), dwSize) != dwSize) {
			return {};
		}

		const NameTable nameTable{{pData.get(), dwSize}};
		for (const auto& record: nameTable.Records()) {
			if (!IsFamilyName(record)) continue;
			if (GetLanguage(record) != langId) continue;

			if (std::wstring name = nameTable.GetString(record); !name.empty()) {
				return name;
			}
		}

		return {};
	}
}