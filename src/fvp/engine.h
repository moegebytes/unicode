#pragma once

#include "..\pch.h"

#ifndef FVP_GAME_ID
	#error Missing FVP_GAME_ID declaration; check your preprocessor definitions!
#endif

namespace FAVS {
	template <typename T>
	inline T& Field(void* base, uintptr_t offset) {
		return *reinterpret_cast<T*>(static_cast<char*>(base) + offset);
	}

	enum RenderState: DWORD {
		Windowed = 0,
		Fullscreen = 1,
		Exclusive = 2
	};

	class Engine {
	public:
#if FVP_GAME_ID <= HAPPY_MARGUERITE
		static const uintptr_t InitWindow = 0x1D5B0; // HWND __thiscall(Engine*)
		static const uintptr_t PrimaryWindowProcA = 0x1CF00; // LRESULT __thiscall(Engine*, HWND, UINT, WPARAM, LPARAM)
#elif FVP_GAME_ID <= HOSHIMEMO_EH
		static const uintptr_t InitWindow = 0x3E590; // HWND __thiscall(Engine*)
		static const uintptr_t PrimaryWindowProcA = 0x3E250; // LRESULT __thiscall(Engine*, HWND, UINT, WPARAM, LPARAM)
		static const uintptr_t GameWindowProcA = 0x3E000; // LRESULT __thiscall(Engine*, HWND, UINT, WPARAM, LPARAM)
#elif FVP_GAME_ID == IROSEKA_WORLD || FVP_GAME_ID == IROHIKA_WORLD || FVP_GAME_ID == AKASEKA_WELWLF
		static const uintptr_t InitWindow = 0x41760; // HWND __thiscall(Engine*)
		static const uintptr_t PrimaryWindowProcA = 0x41390; // LRESULT __thiscall(Engine*, HWND, UINT, WPARAM, LPARAM)
		static const uintptr_t GameWindowProcA = 0x41170; // LRESULT __thiscall(Engine*, HWND, UINT, WPARAM, LPARAM)
#elif FVP_GAME_ID >= ASTRALAIR_WE
		static const uintptr_t InitWindow = 0x434D0; // HWND __thiscall(Engine*)
		static const uintptr_t PrimaryWindowProcA = 0x43100; // LRESULT __thiscall(Engine*, HWND, UINT, WPARAM, LPARAM)
		static const uintptr_t GameWindowProcA = 0x42EE0; // LRESULT __thiscall(Engine*, HWND, UINT, WPARAM, LPARAM)
#endif

		struct Fields {
#if FVP_GAME_ID <= HAPPY_MARGUERITE
			static const uintptr_t PrimaryWindowHnd = 0x08; // HWND
			static const uintptr_t GameW = 0x28; // DWORD
			static const uintptr_t GameH = 0x2C; // DWORD
			static const uintptr_t RenderState = 0x30; // BYTE
			static const uintptr_t Render = 0x38; // Render*
			static const uintptr_t DwStyle = 0xC8; // DWORD
			static const uintptr_t LocalDataPath = 0xAF6B8; // const char*
#elif FVP_GAME_ID <= HOSHIMEMO_EH
			static const uintptr_t PrimaryWindowHnd = 0x0C; // HWND
			static const uintptr_t GameWindowHnd = 0x10; // HWND
			static const uintptr_t GameW = 0x44; // DWORD
			static const uintptr_t GameH = 0x48; // DWORD
			static const uintptr_t ScreenW = 0x4C; // DWORD
			static const uintptr_t ScreenH = 0x50; // DWORD
			static const uintptr_t RenderState = 0x5C; // DWORD
			static const uintptr_t Render = 0x6C; // Render*
			static const uintptr_t DwStyle = 0xFC; // DWORD
			static const uintptr_t LocalDataPath = 0x10076C; // const char*
#elif FVP_GAME_ID == IROSEKA_WORLD || FVP_GAME_ID == IROHIKA_WORLD || FVP_GAME_ID == AKASEKA_WELWLF
			static const uintptr_t PrimaryWindowHnd = 0x0C; // HWND
			static const uintptr_t GameWindowHnd = 0x10; // HWND
			static const uintptr_t GameW = 0x60; // DWORD
			static const uintptr_t GameH = 0x64; // DWORD
			static const uintptr_t ScreenW = 0x68; // DWORD
			static const uintptr_t ScreenH = 0x6C; // DWORD
			static const uintptr_t RenderState = 0x78; // DWORD
			static const uintptr_t Render = 0x8C; // Render*
			static const uintptr_t DwStyle = 0x11C; // DWORD
			static const uintptr_t LocalDataPath = 0x65DDD4; // const char*
#elif FVP_GAME_ID >= ASTRALAIR_WE
			static const uintptr_t PrimaryWindowHnd = 0x0C; // HWND
			static const uintptr_t GameWindowHnd = 0x10; // HWND
			static const uintptr_t GameW = 0x60; // DWORD
			static const uintptr_t GameH = 0x64; // DWORD
			static const uintptr_t ScreenW = 0x68; // DWORD
			static const uintptr_t ScreenH = 0x6C; // DWORD
			static const uintptr_t RenderState = 0x78; // DWORD
			static const uintptr_t Render = 0x8C; // Render*
			static const uintptr_t DwStyle = 0x11C; // DWORD
			static const uintptr_t LocalDataPath = 0x6A7E64; // const char*
#endif
		};
	};

	class Scene {
	public:
#if FVP_GAME_ID <= HAPPY_MARGUERITE
		static const uintptr_t RegisterSyscalls = 0x19DD0; // int __thiscall(Scene*)
		static const uintptr_t SysMovie = 0x14B20; // char __thiscall(Scene*, Variant[])
		static const uintptr_t SysMovieStop = 0x14CC0; // int __stdcall(Variant[])
		static const uintptr_t SysRand = 0x157F0; // int __thiscall(Scene*, Variant[])
		static const uintptr_t SysTextFontName = 0x18A90; // void __thiscall(Scene*, Variant[])
		static const uintptr_t SysWindowMode = 0x16830; // int __thiscall(Scene*, Variant[])
#elif FVP_GAME_ID <= HOSHIMEMO_EH
		static const uintptr_t RegisterSyscalls = 0x3AB70; // int __thiscall(Scene*)
		static const uintptr_t SysMovie = 0x35180; // char __thiscall(Scene*, Variant[])
		static const uintptr_t SysMovieStop = 0x35340; // int __stdcall(Variant[])
		static const uintptr_t SysRand = 0x35E50; // int __thiscall(Scene*, Variant[])
		static const uintptr_t SysTextFontName = 0x381B0; // void __thiscall(Scene*, Variant[])
		static const uintptr_t SysWindowMode = 0x36E80; // int __thiscall(Scene*, Variant[])
#elif FVP_GAME_ID == IROSEKA_WORLD || FVP_GAME_ID == IROHIKA_WORLD || FVP_GAME_ID == AKASEKA_WELWLF
		static const uintptr_t RegisterSyscalls = 0x3DC40; // int __thiscall(Scene*)
		static const uintptr_t SysMovie = 0x39700; // char __thiscall(Scene*, Variant[])
		static const uintptr_t SysMovieStop = 0x398C0; // int __stdcall(Variant[])
		static const uintptr_t SysRand = 0x35FB0; // int __thiscall(Scene*, Variant[])
		static const uintptr_t SysTextFontName = 0x3BD20; // void __thiscall(Scene*, Variant[])
		static const uintptr_t SysWindowMode = 0x3B2C0; // int __thiscall(Scene*, Variant[])
#elif FVP_GAME_ID >= ASTRALAIR_WE
		static const uintptr_t RegisterSyscalls = 0x3F8D0; // int __thiscall(Scene*)
		static const uintptr_t SysMovie = 0x3B120; // char __thiscall(Scene*, Variant[])
		static const uintptr_t SysMovieStop = 0x3B300; // int __stdcall(Variant[])
		static const uintptr_t SysRand = 0x37A20; // int __thiscall(Scene*, Variant[])
		static const uintptr_t SysTextFontName = 0x43DB30; // void __thiscall(Scene*, Variant[])
		static const uintptr_t SysWindowMode = 0x3D0D0; // int __thiscall(Scene*, Variant[])
#endif
	};

	class FontDraw {
	public:
#if FVP_GAME_ID <= HAPPY_MARGUERITE
		static const uintptr_t CreateFont = 0x08DD0; // HFONT __thiscall(FontDraw*)
#elif FVP_GAME_ID <= HOSHIMEMO_EH
		static const uintptr_t CreateFont = 0x280A0; // HFONT __thiscall(FontDraw*)
#elif FVP_GAME_ID == IROSEKA_WORLD || FVP_GAME_ID == IROHIKA_WORLD || FVP_GAME_ID == AKASEKA_WELWLF
		static const uintptr_t CreateFont = 0x2C200; // HFONT __thiscall(FontDraw*)
#elif FVP_GAME_ID >= ASTRALAIR_WE
		static const uintptr_t CreateFont = 0x2D1F0; // HFONT __thiscall(FontDraw*)
#endif
	};

	class FontEnum {
	public:
#if FVP_GAME_ID <= HAPPY_MARGUERITE
		static const uintptr_t EnumFontFamilies = 0x1D9E0; // int __thiscall(FontEnum*, BYTE charset)
		static const uintptr_t EnumFontFamProcA = 0x1D970; // int __stdcall(LOGFONTA*, TEXTMETRICA*, DWORD, LPARAM)
#elif FVP_GAME_ID <= HOSHIMEMO_EH
		static const uintptr_t EnumFontFamilies = 0x3EB60; // int __thiscall(FontEnum*, BYTE charset)
		static const uintptr_t GetFontName = 0x28050; // int __thiscall(FontEnum*, int index)
		static const uintptr_t EnumFontFamProcA = 0x3EAF0; // int __stdcall(LOGFONTA*, TEXTMETRICA*, DWORD, LPARAM)
#elif FVP_GAME_ID == IROSEKA_WORLD || FVP_GAME_ID == IROHIKA_WORLD || FVP_GAME_ID == AKASEKA_WELWLF
		static const uintptr_t EnumFontFamilies = 0x41C30; // int __thiscall(FontEnum*, BYTE charset)
		static const uintptr_t GetFontName = 0x2C1B0; // int __thiscall(FontEnum*, int index)
		static const uintptr_t EnumFontFamProcA = 0x41B90; // int __stdcall(LOGFONTA*, TEXTMETRICA*, DWORD, LPARAM)
#elif FVP_GAME_ID >= ASTRALAIR_WE
		static const uintptr_t EnumFontFamilies = 0x43BC0; // int __thiscall(FontEnum*, BYTE charset)
		static const uintptr_t GetFontName = 0x2D1A0; // int __thiscall(FontEnum*, int index)
		static const uintptr_t EnumFontFamProcA = 0x43B20; // int __stdcall(LOGFONTA*, TEXTMETRICA*, DWORD, LPARAM)
#endif
	};

	class Render {
	public:
#if FVP_GAME_ID <= HAPPY_MARGUERITE
		static const uintptr_t ResetDevice = 0xB6F0; // char __thiscall(Render*, bool force)
#elif FVP_GAME_ID <= HOSHIMEMO_EH
		static const uintptr_t ResetDevice = 0x2B1B0; // char __thiscall(Render*, bool force)
#elif FVP_GAME_ID == IROSEKA_WORLD || FVP_GAME_ID == IROHIKA_WORLD || FVP_GAME_ID == AKASEKA_WELWLF
		static const uintptr_t ResetDevice = 0x2DE90; // char __thiscall(Render*, bool force)
#elif FVP_GAME_ID >= ASTRALAIR_WE
		static const uintptr_t ResetDevice = 0x2F050; // char __thiscall(Render*, bool force)
#endif

		struct Fields {
			static const uintptr_t D3DDev = 0x08; // IDirect3DDevice9*
#if FVP_GAME_ID <= HAPPY_MARGUERITE
			static const uintptr_t SavedRenderFlag = 0x1DC; // BYTE
			static const uintptr_t RenderFlag = 0x1DD; // BYTE
#elif FVP_GAME_ID <= HOSHIMEMO_EH
			static const uintptr_t SavedRenderFlag = 0x1C0; // DWORD
			static const uintptr_t RenderFlag = 0x1C4; // DWORD
#elif FVP_GAME_ID >= IROSEKA_WORLD
			static const uintptr_t SavedRenderFlag = 0x1F8; // DWORD
			static const uintptr_t RenderFlag = 0x1FC; // DWORD
#endif
		};
	};

	class MoviePlayer {
	public:
#if FVP_GAME_ID <= HAPPY_MARGUERITE
		static const uintptr_t InitFilter = 0x01A90; // bool __thiscall(MoviePlayer*, const char* fname)
		static const uintptr_t Play = 0x01920; // int __thiscall(MoviePlayer*)
		static const uintptr_t Stop = 0x01880; // int __thiscall(MoviePlayer*)
#elif FVP_GAME_ID <= HOSHIMEMO_EH
		static const uintptr_t InitFilter = 0x202C0; // bool __thiscall(MoviePlayer*, const char* fname)
		static const uintptr_t Play = 0x20130; // int __thiscall(MoviePlayer*)
		static const uintptr_t Stop = 0x20090; // int __thiscall(MoviePlayer*)
#elif FVP_GAME_ID == IROSEKA_WORLD || FVP_GAME_ID == IROHIKA_WORLD || FVP_GAME_ID == AKASEKA_WELWLF
		static const uintptr_t InitFilter = 0x222F0; // bool __thiscall(MoviePlayer*, const char* fname)
		static const uintptr_t Play = 0x22150; // int __thiscall(MoviePlayer*)
		static const uintptr_t Stop = 0x22080; // int __thiscall(MoviePlayer*)
#elif FVP_GAME_ID >= ASTRALAIR_WE
		static const uintptr_t InitFilter = 0x22430; // bool __thiscall(MoviePlayer*, const char* fname)
		static const uintptr_t Play = 0x221C0; // int __thiscall(MoviePlayer*)
		static const uintptr_t Stop = 0x220F0; // int __thiscall(MoviePlayer*)
#endif
	};
}
