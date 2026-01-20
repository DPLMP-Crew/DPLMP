#include "D3DHooks.h"
#include "d3d9.h"
#include <safetyhook.hpp>
#include "d3dx9.h"
#include "xxhash.h"
#include "Core.h"

namespace D3DHooks {

    bool ExtractMode = false;

    enum eD3D9 : int {
        CreateDevice = 16
    };

    enum eD3D9Device : int {
        Reset = 16,
        Present = 17,
        EndScene = 42,
        CreatePixelShader = 106,
        SetPixelShader = 107
    };

    static SafetyHookInline createDeviceHook{};
    static SafetyHookInline endSceneHook{};
    static SafetyHookInline setPixelShaderHook{};

    static bool hookedDevice = false;

    static HRESULT __stdcall SetPixelShader_Hook(IDirect3DDevice9* device, IDirect3DPixelShader9* pShader) {
        if (ExtractMode) {
            printf("YARR\n");

            XXH64_hash_t const seed = 0;

            UINT size;

            void* code = nullptr;
            pShader->GetFunction(nullptr, &size);

            code = malloc(size);
            pShader->GetFunction(code, &size);

            XXH64_hash_t hash = XXH64(code, size, seed);

            std::wstring fileLoc = Core::Directory + L"\\" + std::to_wstring(hash) + L".ps";

            FILE* psF = _wfopen(fileLoc.c_str(), L"wb");
            fwrite(code, 1, size, psF);
            fclose(psF);

            free(code);
        }
        return setPixelShaderHook.stdcall<HRESULT>(device, pShader);
    }

    static HRESULT __stdcall EndScene_Hook(IDirect3DDevice9* device) {
        ExtractMode = false;
        HRESULT res = endSceneHook.stdcall<HRESULT>(device);
        //printf("End Scene\n");
        return res;
    }

    static void HookDevice(void** deviceVTable) {
        endSceneHook = safetyhook::create_inline(deviceVTable[eD3D9Device::EndScene], EndScene_Hook);
        setPixelShaderHook = safetyhook::create_inline(deviceVTable[eD3D9Device::SetPixelShader], SetPixelShader_Hook);
    }

    static HRESULT __stdcall CreateDevice_Hook(
        IDirect3D9* self,
        UINT Adapter,
        D3DDEVTYPE DeviceType,
        HWND hFocusWindow,
        DWORD BehaviorFlags,
        D3DPRESENT_PARAMETERS* pParams,
        IDirect3DDevice9** ppDevice)
    {
        HRESULT res = createDeviceHook.stdcall<HRESULT>(self, Adapter, DeviceType, hFocusWindow, BehaviorFlags, pParams, ppDevice);

        if (!hookedDevice && SUCCEEDED(res)) {
            hookedDevice = true;
            IDirect3DDevice9* device = *ppDevice;
            void** deviceVTable = *(void***)device;
            HookDevice(deviceVTable);
        }

        //printf("D3D9 Device Created\n");
        return res;
    }

	void Initialize() {
		IDirect3D9* dummyd3d9 = Direct3DCreate9(D3D_SDK_VERSION);
		void** d3dvtable = *(void***)dummyd3d9;

        createDeviceHook = safetyhook::create_inline(d3dvtable[eD3D9::CreateDevice], CreateDevice_Hook);
	}
}