#pragma once

#include "..\pch.h"
#include <boost\pfr.hpp>

namespace Util {
	template <typename T> requires std::is_aggregate_v<T>
	inline std::wstring Describe(const T& obj) {
		std::wstringstream ss;
		ss << "{";
		boost::pfr::for_each_field_with_name(obj, [&ss](std::string_view name, const auto& value, auto index) {
			if (index.value > 0) ss << ", ";
			ss << name.data() << ": " << value;
		});
		ss << "}";
		return ss.str();
	}
}