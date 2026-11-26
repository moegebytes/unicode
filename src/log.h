#pragma once

namespace Log {
	template <typename T>
	concept Streamable = !std::is_convertible_v<const T&, const char*> && !std::is_convertible_v<const T&, const wchar_t*>
		&& !std::is_convertible_v<const T&, std::string_view>;

	template <typename T>
	struct BoundedText {
		BoundedText(const T* data, int length) : data(data), length(length) {}

		const T* data;
		int length;
	};

	class Stream {
	public:
		explicit Stream(const char* function);
		~Stream();

		Stream(const Stream&) = delete;
		Stream& operator=(const Stream&) = delete;

		Stream& operator<<(const char* text);
		Stream& operator<<(const wchar_t* text);
		Stream& operator<<(std::string_view text);
		Stream& operator<<(std::wstring_view text);
		Stream& operator<<(const BoundedText<char>& text);
		Stream& operator<<(const BoundedText<wchar_t>& text);

		template <Streamable T>
		Stream& operator<<(const T& value) {
			m_stream << value;
			return *this;
		}

	private:
		std::wstringstream m_stream;
	};
}

#define DbgPrint(a) { Log::Stream(__FUNCTION__) << a; }

#ifdef _DEBUG
	#define DbgPrintVerbose(a) DbgPrint(a)
#else
	#define DbgPrintVerbose(a)
#endif