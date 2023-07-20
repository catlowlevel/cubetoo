#pragma once
#include <memory>
#include <vector>

#include "Cheat/Cheat.h"

namespace ui
{
    void Update();
    void Begin();
    void End();
    void ToggleMouse(bool state);
    namespace menu
    {
        void Init();
        void Draw();
        void Toggle();
        bool isOpen();
        inline std::vector<std::pair<const char *, std::unique_ptr<Cheat>>> funcs{};
    }  // namespace menu
};     // namespace ui
