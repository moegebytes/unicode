#pragma once

#include <iostream>

namespace Log {
	template <typename T>
	concept Streamable = !std::is_convertible_v<const T&, const wchar_t*>;

	class Stream {
	public:
		explicit Stream(const char* function);
		~Stream();

		Stream(const Stream&) = delete;
		Stream& operator=(const Stream&) = delete;

		Stream& operator<<(const wchar_t* text);
		Stream& operator<<(std::wstring_view text);
		Stream& operator<<(std::wostream& (*manipulator)(std::wostream&));

		template <Streamable T>
		Stream& operator<<(const T& value) {
			m_stream << value;
			return *this;
		}

	private:
		std::wstringstream m_stream;
	};
}

#define OutPrint(a) { Log::Stream(__FUNCTION__) << a; }

#ifdef _DEBUG 
	#define OutPrintVerbose(a) OutPrint(a)
#else
	#define OutPrintVerbose(a)
#endif