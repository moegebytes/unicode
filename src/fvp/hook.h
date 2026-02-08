#pragma once

#include "..\pch.h"

namespace FVP {
	class Hook {
	public:
		static void Install();
	private:
		class MoviePlayer {
		public:
			bool static __thiscall InitFilter(void* instance, const char* fname);
		};
	};
}