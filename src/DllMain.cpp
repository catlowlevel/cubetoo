#include <Windows.h>
#include <stdio.h>

#include <iostream>
#include <thread>

#include "Game.h"
#include "Hooks.h"
#include "Structs.h"
#include "Util.h"

using namespace std;
HookManager* hookManager;

bool exiting = false;
DWORD WINAPI DllThread(LPVOID lpParam)
{
    {
        util::Console con;
        hookManager = new HookManager();

        game::InitVars();
        HookManager::HookSwapBuffers();

        while (!exiting)
        {
            // if (GetAsyncKeyState(VK_DELETE) || GetAsyncKeyState(VK_END))
            //     exiting = true;

            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        delete hookManager;
    }
    FreeLibraryAndExitThread(RCAST<HMODULE>(lpParam), 0);
}

BOOLEAN WINAPI DllMain(IN HINSTANCE hInstance, IN DWORD dwReason, IN LPVOID lpReserved)
{
    UNREFERENCED_PARAMETER(lpReserved);
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hInstance);
        auto handle = CreateThread(0, 0, DllThread, hInstance, 0, 0);
        if (handle)
            CloseHandle(handle);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
    }

    return TRUE;
}
