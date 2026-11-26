#include "pch.h"
#include "util\text.h"

namespace Log {
	Stream::Stream(const char* function) {
		*this << "[" << function << "] ";
	}

	Stream::~Stream() {
		std::wcout << m_stream.str() << std::endl;
	}

	Stream& Stream::operator<<(const wchar_t* text) {
		return *this << (text ? std::wstring_view(text) : std::wstring_view(L"<NULL>"));
	}

	Stream& Stream::operator<<(std::wstring_view text) {
		m_stream << text;
		return *this;
	}

	Stream& Stream::operator<<(std::wostream& (*manipulator)(std::wostream&)) {
		m_stream << manipulator;
		return *this;
	}
}