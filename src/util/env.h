#pragma once

#include "..\pch.h"

namespace Util {
  inline bool IsWineEnvironment() {
    return GetProcAddress(GetModuleHandle(L"ntdll.dll"), "wine_get_version") != nullptr;
  }
}