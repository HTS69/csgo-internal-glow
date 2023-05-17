#include "pch.h"
#include <Windows.h>
#include <iostream>

namespace offsets
{
    // https://github.com/frk1/hazedumper/blob/master/csgo.hpp
    constexpr auto dwLocalPlayer = 0xDE997C;
    constexpr auto dwEntityList = 0x4DFEF0C;
    constexpr auto dwGlowObjectManager = 0x5359988;
    constexpr auto m_iTeamNum = 0xF4;
    constexpr auto m_iGlowIndex = 0x10488;
}

DWORD WINAPI MainThread(HMODULE hModule)
{
    std::cout << "CS:GO Internal Glow / By HTS / cheatglobal.com" << std::endl;
    system("color a");

    DWORD moduleBase = (DWORD)GetModuleHandle("client.dll");

    while (!GetAsyncKeyState(VK_END))
    {
        DWORD LocalPlayer = *(DWORD*)(moduleBase + offsets::dwLocalPlayer);
        DWORD GlowObjectManager = *(DWORD*)(moduleBase + offsets::dwGlowObjectManager);
        DWORD EntityList = *(DWORD*)(moduleBase + offsets::dwEntityList);

        int localTeam = *(int*)(LocalPlayer + offsets::m_iTeamNum);

        for (int i = 1; i < 32; i++)
        {
            DWORD entity = *(DWORD*)((moduleBase + offsets::dwEntityList) * i * 0x10);

            if (entity == NULL) continue;

            int glowIndex = *(int*)(entity + offsets::m_iGlowIndex);
            int entityTeam = *(int*)(entity + offsets::m_iTeamNum);

            if (entityTeam == localTeam)
            {
                //Local Team
                *(float*)((GlowObjectManager + glowIndex * 0x38 + 0x4)) = 0.f;
                *(float*)((GlowObjectManager + glowIndex * 0x38 + 0x8)) = 1.f;
                *(float*)((GlowObjectManager + glowIndex * 0x38 + 0xC)) = 0.f;
                *(float*)((GlowObjectManager + glowIndex * 0x38 + 0x10)) = 1.7f;
            }
            else
            {
                //Enemy Team
                *(float*)((GlowObjectManager + glowIndex * 0x38 + 0x4)) = 0.f;
                *(float*)((GlowObjectManager + glowIndex * 0x38 + 0x8)) = 1.f;
                *(float*)((GlowObjectManager + glowIndex * 0x38 + 0xC)) = 0.f;
                *(float*)((GlowObjectManager + glowIndex * 0x38 + 0x10)) = 1.7f;
            }
            *(bool*)((GlowObjectManager + glowIndex * 0x38 + 0x24)) = true;
            *(bool*)((GlowObjectManager + glowIndex * 0x38 + 0x25)) = false;
        }
    }

    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}