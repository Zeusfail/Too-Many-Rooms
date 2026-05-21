#include "pch.h"
#include <Windows.h>
#include <cstdint>
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
    if (!hMod) hMod = GetModuleHandleA("Subnautica2-Win64-Shipping.exe");
    if (!hMod) { OutputDebugStringA("[NRC] Module introuvable\n"); return; }

    uintptr_t base = (uintptr_t)hMod;
    char buf[256];

    static const uint8_t patch1[7] = { 0xC6, 0x81, 0x48, 0x01, 0x00, 0x00, 0x01 };
    static const uint8_t patch2[7] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };

    // Pattern scan — find both byte sequences in all committed non-guarded regions
    uintptr_t p1 = 0; // pattern: 40 88 B9 48 01 00 00
    uintptr_t p2 = 0; // pattern: 41 88 B7 48 01 00 00

    MEMORY_BASIC_INFORMATION mbi = {};
    uintptr_t addr = base;
    while (VirtualQuery((void*)addr, &mbi, sizeof(mbi)))
    {
        if (mbi.State == MEM_COMMIT && !(mbi.Protect & PAGE_NOACCESS) && !(mbi.Protect & PAGE_GUARD))
        {
            uint8_t* start = (uint8_t*)mbi.BaseAddress;
            SIZE_T size = mbi.RegionSize;
            for (SIZE_T i = 0; i < size - 7; i++)
            {
                if (!p1 && memcmp(start + i, "\x40\x88\xB9\x48\x01\x00\x00", 7) == 0)
                    p1 = (uintptr_t)(start + i);
                if (!p2 && memcmp(start + i, "\x41\x88\x87\x48\x01\x00\x00", 7) == 0)
                    p2 = (uintptr_t)(start + i);
            }
        }
        addr += mbi.RegionSize;
    }

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

    if (p1) applyPatch(p1, patch1, 7, "collision->1");
    else OutputDebugStringA("[NRC] Pattern 1 NOT found\n");

    if (p2) applyPatch(p2, patch2, 7, "surface->NOP");
    else OutputDebugStringA("[NRC] Pattern 2 NOT found\n");
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