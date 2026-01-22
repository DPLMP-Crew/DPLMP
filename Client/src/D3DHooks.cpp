#include "D3DHooks.h"
#include "d3d9.h"
#include <safetyhook.hpp>
#include "d3dx9.h"
#include "xxhash.h"
#include "Core.h"
#include "d3dcompiler.h"
#include <filesystem>
#include <fstream>

namespace D3DHooks {

    bool ExtractMode = false;

    enum eD3D9 : int {
        CreateDevice = 16
    };

    enum eD3D9Device : int {
        Reset = 16,
        Present = 17,
        EndScene = 42,
        CreateVertexShader = 91,
        CreatePixelShader = 106,
        SetPixelShader = 107
    };

    static SafetyHookInline createDeviceHook{};
    static SafetyHookInline endSceneHook{};
    static SafetyHookInline createPixelShaderHook{};

    static XXH64_hash_t const seed = 0;
    static bool hookedDevice = false;

    static size_t GetShaderSize(const DWORD* code) {
        size_t i = 0;
        while (code[i] != D3DSIO_END) {
            i++;
        }
        return (i + 1) * sizeof(DWORD);
    }

    static std::string ReadTextFile(const std::wstring& path)
    {
        std::ifstream file(path);
        file.exceptions(std::ios::failbit | std::ios::badbit);

        return std::string(
            (std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>()
        );
    }

    static HRESULT __stdcall CreatePixelShader_Hook(IDirect3DDevice9* device, DWORD* pFunction, IDirect3DPixelShader9** ppShader) {
        size_t size = GetShaderSize(pFunction);
        XXH64_hash_t hash = XXH64(pFunction, size, seed);
        std::wstring fileLoc = Core::Directory + L"\\data\\shaders\\" + std::to_wstring(hash) + L".hlsl";
        if (std::filesystem::exists(fileLoc))
        {
            std::string hlsl = ReadTextFile(fileLoc);
            ID3DBlob* compiledShader;
            ID3DBlob* errors;
            HRESULT compileResult = D3DCompile(hlsl.c_str(), hlsl.size(), "Shader", NULL, NULL, "main", "ps_3_0", 0 , 0, &compiledShader, &errors);
            if (!SUCCEEDED(compileResult)) {
                printf("Shader compilation FAILED!\n");
                printf((const char*)errors->GetBufferPointer());
                printf("\n");
            }
            HRESULT result = createPixelShaderHook.stdcall<HRESULT>(device, compiledShader->GetBufferPointer(), ppShader);
            compiledShader->Release();
            errors->Release();
            compiledShader = nullptr;
            errors = nullptr;
            return result;
        }
        return createPixelShaderHook.stdcall<HRESULT>(device, pFunction, ppShader);
    }

    static HRESULT __stdcall EndScene_Hook(IDirect3DDevice9* device) {
        ExtractMode = false;
        HRESULT res = endSceneHook.stdcall<HRESULT>(device);
        return res;
    }

    static void HookDevice(void** deviceVTable) {
        endSceneHook = safetyhook::create_inline(deviceVTable[eD3D9Device::EndScene], EndScene_Hook);
        createPixelShaderHook = safetyhook::create_inline(deviceVTable[eD3D9Device::CreatePixelShader], CreatePixelShader_Hook);
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

        return res;
    }

	void Initialize() {
		IDirect3D9* dummyd3d9 = Direct3DCreate9(D3D_SDK_VERSION);
		void** d3dvtable = *(void***)dummyd3d9;

        createDeviceHook = safetyhook::create_inline(d3dvtable[eD3D9::CreateDevice], CreateDevice_Hook);
	}
}