#include "Util.h"

#include <Windows.h>

namespace util
{
    uint32_t Hash(const char* str) noexcept
    {
        uint32_t hash = HASH_VALUE;
        while (*str != '\0')
        {
            hash ^= static_cast<uint32_t>(*str++);
            hash *= HASH_VALUE;
        }
        return hash;
    }

    uint32_t Hash(const std::string_view& str) noexcept
    {
        return Hash(str.data());
    }

    bool Console::initialized = false;
    Console::Console()
    {
        if (Console::initialized)
        {
            printf("Console already initialized\n");
            return;
        }
        Console::initialized = true;
        AllocConsole();
        freopen_s(&file, "CONOUT$", "w", stdout);
        printf("Console initialized\n");
    }
    Console::~Console()
    {
        if (Console::initialized)
        {
            printf("Console uinitialized\n");
            if (file)
                fclose(file);
            FreeConsole();
        }
    }
};  // namespace util
