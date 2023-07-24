#pragma once
#include <Windows.h>

#include <unordered_map>

#include "Util.h"

class Hooker
{
  public:
    Hooker(const char *name, void *fnTarget, void *fnDetour);
    bool Enable();
    bool Disable();
    bool Remove();
    bool Create();

    template <typename T = void *>
    T GetOriginal()
    {
        return RCAST<T>(fnOriginal);
    }

  private:
    friend class HookManager;
    bool bCreated;
    void *fnTarget;
    void *fnDetour;
    void *fnOriginal;
    const char *name;
};

class HookManager
{
  public:
    HookManager(HookManager &&) = delete;
    HookManager(const HookManager &other) = delete;
    HookManager &operator=(const HookManager &other) = delete;

    HookManager();
    ~HookManager();

    Hooker *CreateAndEnable(const char *name, void *fnTarget, void *fnDetour);

    // hashed = util::Hash(name)
    bool DisableAndRemove(const uint32_t hashed);

    // hashed = util::Hash(name)
    Hooker *GetHooker(const uint32_t hashed);

    static void HookSwapBuffers();
    static void HookWindowProc(HWND hWnd);

  private:
    bool RemoveAll_();
    static int instanceCount_;
    std::unordered_map<uint32_t, Hooker> hooks_;
};

extern HookManager *hookManager;

#define HM_CE(fnTarget, fnDetour)                                                                                      \
    hookManager->CreateAndEnable(#fnTarget, RCAST<void *>(fnTarget), RCAST<void *>(fnDetour))
#define HM_DR(fnTarget) hookManager->DisableAndRemove(#fnTarget)

constexpr auto SWAPBUFFERS_HASH = util::CT_Hash("swapBuffers");
constexpr auto WINDOWPROC_HASH = util::CT_Hash("windowProc");
