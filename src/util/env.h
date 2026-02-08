#pragma once

#include "..\pch.h"

namespace Util {
  inline bool IsWineEnvironment() {
    const auto hNtdll = GetModuleHandle(L"ntdll.dll");
    if (GetProcAddress(hNtdll, "wine_get_version") != nullptr) {
      return true;
    }

    return false;
  }
}