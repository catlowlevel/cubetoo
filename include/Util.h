#pragma once
#include <Windows.h>

#include <cstdint>
#include <functional>
#include <string>

constexpr uint32_t HASH_VALUE = 0x1000193;

namespace util
{
    // FNV
    constexpr uint32_t CT_Hash(const char *str, uint32_t value = HASH_VALUE) noexcept
    {
        return (*str == '\0') ? value : CT_Hash(str + 1, (value ^ uint32_t(*str)) * HASH_VALUE);
    }
    // FNV
    constexpr uint32_t CT_Hash(std::string_view str) noexcept
    {
        return CT_Hash(str.data());
    }
    // FNV
    uint32_t Hash(const char *str) noexcept;
    // FNV
    uint32_t Hash(const std::string_view &str) noexcept;

    template <typename T>
    std::string to_sring(T val)
    {
        return std::to_string(val);
    }

    template <typename T>
    std::wstring to_wsring(T val)
    {
        return std::to_wstring(val);
    }

    template <typename T>
    T GetExportFunction(const std::wstring_view &module, const std::string_view &exportFn)
    {
        return RCAST<T>(GetProcAddress(GetModuleHandle(module.data()), exportFn.data()));
    }

    class Console
    {
      public:
        Console();
        ~Console();

      private:
        FILE *file;
        static bool initialized;
    };

};  // namespace util

template <typename To, typename From>
constexpr auto RCAST(From from)
{
    return reinterpret_cast<To>((from));
}
