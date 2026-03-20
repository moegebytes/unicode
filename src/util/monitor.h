#pragma once

#include "..\pch.h"

namespace Util {
	inline SIZE GetMonitorSize(HWND hWnd) {
		MONITORINFO mi = { sizeof(mi) };
		if (!GetMonitorInfoA(MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST), &mi)) {
			return { GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
		}

		return {
			mi.rcMonitor.right - mi.rcMonitor.left,
			mi.rcMonitor.bottom - mi.rcMonitor.top
		};
	}
}