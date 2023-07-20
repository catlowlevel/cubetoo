#pragma once
#include <Windows.h>

#include <functional>
#include <type_traits>

// Thx ChatGPT
template <typename T>
concept IsCharPointer = std::is_same_v<T, char *> || std::is_same_v<T, const char *> ||
                        std::is_same_v<T, signed char *> || std::is_same_v<T, const signed char *> ||
                        std::is_same_v<T, unsigned char *> || std::is_same_v<T, const unsigned char *>;

namespace mem
{

    template <IsCharPointer T>
    bool WriteBytes(void *src, T bytes, size_t size)
    {
        unsigned long OldProtection;
        VirtualProtect(src, size, PAGE_EXECUTE_READWRITE, &OldProtection);
        memcpy(src, bytes, size);
        VirtualProtect(src, size, OldProtection, &OldProtection);
        return true;
    }

    void PauseThread();
    void UnpauseThread();

    template <typename Func, typename... Args>
        requires std::invocable<Func, Args...>
    void ExecuteInMainthread(Func &&callback, Args &&...args)
    {
        PauseThread();
        std::invoke(std::forward<Func>(callback), std::forward<Args>(args)...);
        UnpauseThread();
    }

}  // namespace mem
