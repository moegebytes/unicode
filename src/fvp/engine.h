#pragma once

#include "..\pch.h"

#ifndef FVP_ENGINE_VER
	#error Missing FVP_ENGINE_VER declaration; check your preprocessor definitions!
#endif

namespace FAVS {
	class Engine {
	public:
#if FVP_ENGINE_VER == 10000
		static const uintptr_t InitWindow = 0x1D5B0;
		static const uintptr_t PrimaryWindowProcA = 0x1CF00;
#elif FVP_ENGINE_VER == 10002
		static const uintptr_t InitWindow = 0x3E590;
		static const uintptr_t PrimaryWindowProcA = 0x3E250;
		static const uintptr_t GameWindowProcA = 0x3E000;
#elif FVP_ENGINE_VER == 10095
		static const uintptr_t InitWindow = 0x434D0;
		static const uintptr_t PrimaryWindowProcA = 0x43100;
		static const uintptr_t GameWindowProcA = 0x42EE0;
#endif
	};

	class Scene {
	public:
#if FVP_ENGINE_VER == 10000
		static const uintptr_t RegisterSyscalls = 0x19DD0;
		static const uintptr_t SysMovie = 0x14B20;
		static const uintptr_t SysMovieStop = 0x14CC0;
		static const uintptr_t SysRand = 0x157F0;
		static const uintptr_t SysTextFontName = 0x18A90;
		static const uintptr_t SysWindowMode = 0x16830;
#elif FVP_ENGINE_VER == 10002
		static const uintptr_t RegisterSyscalls = 0x3AB70;
		static const uintptr_t SysMovie = 0x35180;
		static const uintptr_t SysMovieStop = 0x35340;
		static const uintptr_t SysRand = 0x35E50;
		static const uintptr_t SysTextFontName = 0x381B0;
		static const uintptr_t SysWindowMode = 0x36E80;
#elif FVP_ENGINE_VER == 10095
		static const uintptr_t RegisterSyscalls = 0x3F8D0;
		static const uintptr_t SysMovie = 0x3B120;
		static const uintptr_t SysMovieStop = 0x3B300;
		static const uintptr_t SysRand = 0x37A20;
		static const uintptr_t SysTextFontName = 0x43DB30;
		static const uintptr_t SysWindowMode = 0x3D0D0;
#endif
	};

	class FontDraw {
	public:
#if FVP_ENGINE_VER == 10000
		static const uintptr_t CreateFont = 0x08DD0;
#elif FVP_ENGINE_VER == 10002
		static const uintptr_t CreateFont = 0x280A0;
#elif FVP_ENGINE_VER == 10095
		static const uintptr_t CreateFont = 0x2D1F0;
#endif
	};

	class FontEnum {
	public:
#if FVP_ENGINE_VER == 10000
		static const uintptr_t EnumFontFamilies = 0x1D9E0;
		static const uintptr_t EnumFontFamProcA = 0x1D970;
#elif FVP_ENGINE_VER == 10002
		static const uintptr_t EnumFontFamilies = 0x3EB60;
		static const uintptr_t GetFontName = 0x28050;
		static const uintptr_t EnumFontFamProcA = 0x3EAF0;
#elif FVP_ENGINE_VER == 10095
		static const uintptr_t EnumFontFamilies = 0x43BC0;
		static const uintptr_t GetFontName = 0x2D1A0;
		static const uintptr_t EnumFontFamProcA = 0x43B20;
#endif
	};

	class MoviePlayer {
	public:
#if FVP_ENGINE_VER == 10000
		static const uintptr_t InitFilter = 0x01A90;
		static const uintptr_t Play = 0x01920;
		static const uintptr_t Stop = 0x01880;
#elif FVP_ENGINE_VER == 10002
		static const uintptr_t InitFilter = 0x202C0;
		static const uintptr_t Play = 0x20130;
		static const uintptr_t Stop = 0x20090;
#elif FVP_ENGINE_VER == 10095
		static const uintptr_t InitFilter = 0x22430;
		static const uintptr_t Play = 0x221C0;
		static const uintptr_t Stop = 0x220F0;
#endif
	};
}