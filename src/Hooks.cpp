#include "Hooks.h"

#include <minhook/MinHook.h>
#include <winuser.h>

#include <cassert>
#include <iostream>

#include "Functions.h"
#include "Memory.h"
#include "Util.h"

using namespace std;

#define MH_LOG(func)                                                                                                   \
    if ((func) == MH_OK)                                                                                               \
        std::cout << name << " SUCCESS\n";                                                                             \
    else                                                                                                               \
    {                                                                                                                  \
        std::cout << name << " FAILED\n";                                                                              \
        return false;                                                                                                  \
    }

Hooker::Hooker(const char *name, void *fnTarget, void *fnDetour)
    : bCreated(false), fnTarget(fnTarget), fnDetour(fnDetour), fnOriginal(nullptr), name(name)
{
}

bool Hooker::Enable()
{
    if (!bCreated)
        return false;
    cout << "Enable ";
    MH_LOG(MH_EnableHook(fnTarget));
    return true;
}

bool Hooker::Disable()
{
    if (!bCreated)
        return false;
    cout << "Disable ";
    MH_LOG(MH_DisableHook(fnTarget));
    return true;
}

bool Hooker::Remove()
{
    if (!bCreated)
        return false;
    cout << "Remove ";
    MH_LOG(MH_RemoveHook(fnTarget));
    return true;
}

bool Hooker::Create()
{
    cout << "Create ";
    MH_LOG(MH_CreateHook(fnTarget, fnDetour, (LPVOID *)&fnOriginal));
    bCreated = true;
    return true;
}

int HookManager::instanceCount_ = 0;
HookManager::HookManager()
{
    if (++HookManager::instanceCount_ > 1)
        throw std::runtime_error("Only 1 instance of Hook Manager is allowed!");
    MH_Initialize();
}

HookManager::~HookManager()
{
    mem::ExecuteInMainthread(&HookManager::RemoveAll_, this);
    // RemoveAll_();
    MH_Uninitialize();
}

Hooker *HookManager::CreateAndEnable(const char *name, void *fnTarget, void *fnDetour)
{
    auto hashed = util::CT_Hash(name);
    hooks_.emplace(hashed, Hooker(name, fnTarget, fnDetour));
    auto &hook = hooks_.at(hashed);
    if (!hook.Create())
        return nullptr;
    if (!hook.Enable())
        return nullptr;
    return &hook;
}

bool HookManager::DisableAndRemove(const uint32_t hashed)
{
    auto it = hooks_.find(hashed);
    if (it == hooks_.end())
        return false;
    if (!it->second.Create())
        return false;
    if (!it->second.Enable())
        return false;
    return true;
}

Hooker *HookManager::GetHooker(const uint32_t hashed)
{
    auto it = hooks_.find(hashed);
    if (it == hooks_.end())
        return nullptr;
    return &it->second;
}
bool HookManager::RemoveAll_()
{
    bool result = true;
    for (auto &[key, hook] : hooks_)
    {
        result &= hook.Disable();
        result &= hook.Remove();
    }
    return result;
}

void HookManager::HookSwapBuffers()
{
    auto opengl32 = GetModuleHandle(L"opengl32.dll");
    if (!opengl32)
    {
        cout << "Can't get handle to opengl32.dll\n";
        return;
    }
    LPVOID swapBuffers = RCAST<LPVOID>(GetProcAddress(opengl32, "wglSwapBuffers"));
    cout << "wglSwapBuffers 0x" << hex << swapBuffers << endl;
    assert(SWAPBUFFERS_HASH == util::CT_Hash("swapBuffers"));
    HM_CE(swapBuffers, hk_SwapBuffers);
}

void HookManager::HookWindowProc(HWND hWnd)
{
    LPVOID windowProc = RCAST<LPVOID>(GetWindowLongPtr(hWnd, GWLP_WNDPROC));
    assert(WINDOWPROC_HASH == util::CT_Hash("windowProc"));
    HM_CE(windowProc, hk_WinProc);
}
