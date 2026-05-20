#include "pch.h"
#include <Windows.h>
#include <cstdlib>
#include <cstdio>

typedef void(__cdecl* VFunc)(void*);

struct MyVTable {
    void* destructor;
    VFunc on_program_start;
    VFunc on_cpp_mods_loaded;
    VFunc on_ui_init;
    VFunc on_unreal_init;
    VFunc on_update;
    VFunc on_render_tab;
    void* on_dll_load;
    void* on_lua_start1;
    void* on_lua_start2;
    void* on_lua_start3;
    void* on_lua_start4;
    void* on_lua_stop1;
    void* on_lua_stop2;
    void* on_lua_stop3;
    void* on_lua_stop4;
};

static void my_on_unreal_init(void* self)
{
    OutputDebugStringA("[NRC] on_unreal_init!\n");

    HMODULE hMod = GetModuleHandleA("Subnautica2-WinGDK-Shipping.exe");
    if (!hMod) { OutputDebugStringA("[NRC] Module introuvable\n"); return; }

    uintptr_t base = (uintptr_t)hMod;
    char buf[256];

    // Patch 1: base+0x6818F48 — MOV [RCX+148h], DIL (writes 0 on collision)
    //          -> MOV BYTE PTR [RCX+148h], 1  (always allow placement)
    uintptr_t p1 = base + 0x6818F48;
    static const uint8_t patch1[7] = { 0xC6, 0x81, 0x48, 0x01, 0x00, 0x00, 0x01 };

    // Patch 2: base+0x6818FD2 — MOV [R15+148h], DIL (writes 0 on invalid surface)
    //          -> NOP x7
    uintptr_t p2 = base + 0x6818FD2;
    static const uint8_t patch2[7] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };

    DWORD oldProt = 0;
    auto applyPatch = [&](uintptr_t addr, const uint8_t* patch, size_t len, const char* name)
    {
        if (VirtualProtect((void*)addr, len, PAGE_EXECUTE_READWRITE, &oldProt))
        {
            memcpy((void*)addr, patch, len);
            VirtualProtect((void*)addr, len, oldProt, &oldProt);
            FlushInstructionCache(GetCurrentProcess(), (void*)addr, len);
            sprintf_s(buf, "[NRC] Patch %s applique a %p\n", name, (void*)addr);
            OutputDebugStringA(buf);
        }
        else
        {
            sprintf_s(buf, "[NRC] Patch %s echoue a %p\n", name, (void*)addr);
            OutputDebugStringA(buf);
        }
    };

    applyPatch(p1, patch1, 7, "0x6818F48 (collision->1)");
    applyPatch(p2, patch2, 7, "0x6818FD2 (surface->NOP)");
}

static void noop(void* self) {}

static MyVTable g_vtable = {
    (void*)noop,
    noop, noop, noop,
    my_on_unreal_init,
    noop, noop,
    (void*)noop,
    (void*)noop, (void*)noop, (void*)noop, (void*)noop,
    (void*)noop, (void*)noop, (void*)noop, (void*)noop,
};

struct FakeMod {
    MyVTable* vtable;
    void* ModName;
    void* ModVersion;
    void* ModDescription;
    void* ModAuthors;
};

extern "C" __declspec(dllexport) FakeMod* start_mod()
{
    OutputDebugStringA("[NRC] start_mod!\n");
    FakeMod* mod = (FakeMod*)malloc(sizeof(FakeMod));
    mod->vtable = &g_vtable;
    mod->ModName = nullptr;
    mod->ModVersion = nullptr;
    mod->ModDescription = nullptr;
    mod->ModAuthors = nullptr;
    return mod;
}

extern "C" __declspec(dllexport) void uninstall_mod(FakeMod* mod)
{
    if (mod) free(mod);
}

BOOL APIENTRY DllMain(HMODULE h, DWORD r, LPVOID) { return TRUE; }