#include "Memory.h"

#include "Game.h"
#include "Util.h"

namespace mem
{
    // sauerbraten.exe+1A62C5 - jmp sauerbraten.exe+1A6160
    constexpr auto addyOffset = 0x1A62C5;
    void PauseThread()
    {
        auto addy = game::ModuleOffset<void *>(addyOffset);
        mem::WriteBytes(addy, "\xEB\xFE\x90\x90\x90", 5);
    }
    void UnpauseThread()
    {
        auto addy = game::ModuleOffset<void *>(addyOffset);
        mem::WriteBytes(addy, "\xE9\x96\xFE\xFF\xFF", 5);
    }
}  // namespace mem
