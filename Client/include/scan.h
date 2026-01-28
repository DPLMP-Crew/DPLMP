#pragma once
#include <Windows.h>
#if LDSCAN_SAFETYHOOK
#include <safetyhook.hpp>
#else
#include "MinHook.h"
#endif

DWORD GetMainThreadId();
void WriteToMemory(DWORD addressToWrite, void* valueToWrite, int byteNum);
char* ScanBasic(char* pattern, char* mask, char* begin, int size);
char* ScanInternal(char* pattern, char* mask, char* begin, int size);
void Nop(BYTE* pAddress, DWORD dwLen);
void MakeJMP(BYTE* pAddress, DWORD dwJumpTo, DWORD dwLen);

enum class InjectionResult {
	Success,
	Failed
};

class InlineHook {
public:
	InjectionResult m_Result = InjectionResult::Success;
	InlineHook(){}
	InlineHook(InjectionResult failure) { m_Result = failure; }
#if LDSCAN_SAFETYHOOK
	InlineHook(SafetyHookInline safetyHook) {
		m_SafetyHook = safetyHook;
	}

	template<typename Ret = void, typename... Args>
	Ret stdcall(Args... args) const {
		return m_SafetyHook.stdcall<Ret>(args...);
	}

	template<typename Ret = void, typename... Args>
	Ret ccall(Args... args) const {
		return m_SafetyHook.ccall<Ret>(args...);
	}

	template<typename Ret = void, typename... Args>
	Ret thiscall(Args... args) const {
		return m_SafetyHook.thiscall<Ret>(args...);
	}

	template<typename Ret = void, typename... Args>
	Ret fastcall(Args... args) const {
		return m_SafetyHook.fastcall<Ret>(args...);
	}

#else
	InlineHook(void* address) {
		m_Address = address;
	}

	template<typename Ret = void, typename... Args>
	Ret stdcall(Args... args) const {
		using FnType = Ret(__stdcall*)(Args...);
		if constexpr (std::is_void_v<Ret>) {
			reinterpret_cast<FnType>(m_Address)(args...);
		}
		else {
			return reinterpret_cast<FnType>(m_Address)(args...);
		}
	}

	template<typename Ret = void, typename... Args>
	Ret ccall(Args... args) const {
		using FnType = Ret(__cdecl*)(Args...);
		if constexpr (std::is_void_v<Ret>) {
			reinterpret_cast<FnType>(m_Address)(args...);
		}
		else {
			return reinterpret_cast<FnType>(m_Address)(args...);
		}
	}

	template<typename Ret = void, typename... Args>
	Ret thiscall(Args... args) const {
		using FnType = Ret(__thiscall*)(Args...);
		if constexpr (std::is_void_v<Ret>) {
			reinterpret_cast<FnType>(m_Address)(args...);
		}
		else {
			return reinterpret_cast<FnType>(m_Address)(args...);
		}
	}

	template<typename Ret = void, typename... Args>
	Ret fastcall(Args... args) const {
		using FnType = Ret(__fastcall*)(Args...);
		if constexpr (std::is_void_v<Ret>) {
			reinterpret_cast<FnType>(m_Address)(args...);
		}
		else {
			return reinterpret_cast<FnType>(m_Address)(args...);
		}
	}

#endif
	static InlineHook Create(DWORD address, void* target);
	static InlineHook Create(void* address, void* target) { return Create((DWORD)address, target); }
private:
#if LDSCAN_SAFETYHOOK
	SafetyHookInline m_SafetyHook;
#else
	static bool m_Initialized;
	void* m_Address;
#endif
};