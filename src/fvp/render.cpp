#include "..\pch.h"
#include "render.h"
#include "engine.h"

namespace FVP {
	void Render::ResetDevice(void* self) {
		auto fn = (char(__thiscall*)(void*, bool))((uintptr_t)GetModuleHandle(NULL) + FAVS::Render::ResetDevice);
		fn(self, true);
	}
}