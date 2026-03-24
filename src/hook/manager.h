#pragma once

#include "..\pch.h"
#include <detours.h>

class HookManager {
public:
	template <typename Fn>
	static void Install(Fn func, Fn handler) {
		Enable(func, handler);
		HolderMap[reinterpret_cast<void*>(handler)] = reinterpret_cast<void *>(func);
	}

	template <typename Fn>
	static Fn GetOrigin(Fn handler) noexcept {
		auto it = HolderMap.find(reinterpret_cast<void*>(handler));
		if (it == HolderMap.end()) {
			return nullptr;
		}
		return reinterpret_cast<Fn>(it->second);
	}

	template <typename Fn>
	static void Detach(Fn handler) noexcept {
		Disable(handler);
		HolderMap.erase(reinterpret_cast<void *>(handler));
	}

#ifndef _WIN64
	template <typename RType, typename... Params>
	static RType Call(RType (__cdecl *handler)(Params...), Params... params) {
		auto origin = GetOrigin(handler);
		if (origin != nullptr) {
			return origin(params...);
		}
		return RType();
	}

	template <typename RType, typename... Params>
	static RType Call(RType (__stdcall *handler)(Params...), Params... params) {
		auto origin = GetOrigin(handler);
		if (origin != nullptr) {
			return origin(params...);
		}
		return RType();
	}

	template <typename RType, typename... Params>
	static RType Call(RType (__thiscall *handler)(Params...), Params... params) {
		auto origin = GetOrigin(handler);
		if (origin != nullptr) {
			return origin(params...);
		}
		return RType();
	}
#endif

	template <typename RType, typename... Params>
	static RType Call(RType (__fastcall *handler)(Params...), Params... params) {
		auto origin = GetOrigin(handler);
		if (origin != nullptr) {
			return origin(params...);
		}
		return RType();
	}

	template <typename RType, typename... Params>
	static RType Call(RType (__vectorcall *handler)(Params...), Params... params) {
		auto origin = GetOrigin(handler);
		if (origin != nullptr) {
			return origin(params...);
		}
		return RType();
	}

	static void DetachAll() noexcept {
		for (const auto &[key, value] : HolderMap) {
			Disable(key);
		}
		HolderMap.clear();
	}

private:
	inline static std::unordered_map<void*, void*> HolderMap{};

	template <typename Fn>
	static void Disable(Fn handler) noexcept {
		Fn origin = GetOrigin(handler);
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&(void* &)origin, handler);
		DetourTransactionCommit();
	}

	template <typename Fn>
	static void Enable(Fn &func, Fn handler) noexcept {
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(void* &)func, handler);
		DetourTransactionCommit();
	}
};