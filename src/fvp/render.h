#pragma once

#include "..\pch.h"
#include "engine.h"

namespace FVP {
	class Render {
	public:
		inline static void ResetDevice(void* self) {
			// Triggering this function will let engine perform up/downscaling if (GameW,GameH) is higher 
			// than (ScreenW,ScreenH). Normally, this is only executed when entering fullscreen, but we hihack it
			// to enable dynamic window resizing. This executes IDirect3DDevice9::Reset, so it should not be
			// called on events that happen fast, as it's relative expensive operation to perform.
			auto fn = (char(__thiscall*)(void*, bool))((uintptr_t)GetModuleHandle(NULL) + FAVS::Render::ResetDevice);
			fn(self, true);
		}
	};
}