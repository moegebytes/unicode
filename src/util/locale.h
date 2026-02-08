#pragma once

#include "..\pch.h"

namespace Util {
  inline bool IsOSDependantCodePage(unsigned int codePage) {
    switch (codePage) {
    case CP_ACP:
    case CP_MACCP:
    case CP_OEMCP:
    case CP_THREAD_ACP:
      return true;
    default:
      return false;
    }
  }
}