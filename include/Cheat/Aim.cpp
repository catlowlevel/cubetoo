#include "Aim.h"

#include <imgui./imgui.h>

#include "Game.h"

void Aim::Run()
{
    if (bEnabled)
    {
        auto target = game::getClosestEnt(SKIP_SAMETEAM | SKIP_NOT_VISIBLE);
        if (!target)
            return;
        if (target->health <= 0)
            return;
        auto local = game::localPlayer;
        auto delta = target->pos - local->pos;
        auto angle = game::calcAngle(delta);
        angle.x -= 90.f;
        local->angle = angle;
    }
}
void Aim::Menu()
{
    ImGui::Checkbox("Enable", &bEnabled);
}