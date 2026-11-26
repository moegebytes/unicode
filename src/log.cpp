#include "pch.h"
#include "util\text.h"

namespace Log {
	Stream::Stream(const char* function) {
		*this << "[" << function << "] ";
	}

	Stream::~Stream() {
		OutputDebugStringW(m_stream.str().c_str());
	}

	Stream& Stream::operator<<(const char* text) {
		return *this << (text ? Util::NarrowToWide(CP_SHIFT_JIS, text).get() : L"<NULL>");
	}

	Stream& Stream::operator<<(const wchar_t* text) {
		return *this << (text ? std::wstring_view(text) : std::wstring_view(L"<NULL>"));
	}

	Stream& Stream::operator<<(std::string_view text) {
		return *this << std::string(text).c_str();
	}

	Stream& Stream::operator<<(std::wstring_view text) {
		m_stream << text;
		return *this;
	}

	Stream& Stream::operator<<(const BoundedText<char>& text) {
		if (!text.data || text.length < 0) {
			return *this << text.data;
		}

		return *this << std::string_view(text.data, text.length);
	}

	Stream& Stream::operator<<(const BoundedText<wchar_t>& text) {
		if (!text.data || text.length < 0) {
			return *this << text.data;
		}

		return *this << std::wstring_view(text.data, text.length);
	}
}