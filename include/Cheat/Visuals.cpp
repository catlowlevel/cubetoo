#include "Visuals.h"

#include <imgui/imgui_internal.h>

#include <iostream>
#include <numbers>

#include "Draw.h"
#include "Game.h"
#include "Util.h"

using namespace std;

Visuals::Visuals()
{
    bEnabled = true;
}
#define M_PI numbers::pi_v<float>
// Function to convert degrees to radians
constexpr float toRadians(float degrees)
{
    return degrees * M_PI / 180.0f;
}

void vecfromyawpitch(float yaw, float pitch, int move, int strafe, Vec3 &m)
{
    if (move)
    {
        m.x = move * -sinf(toRadians(yaw));
        m.y = move * cosf(toRadians(yaw));
    }
    else
        m.x = m.y = 0;

    if (pitch)
    {
        m.x *= cosf(toRadians(pitch));
        m.y *= cosf(toRadians(pitch));
        m.z = move * sinf(toRadians(pitch));
    }
    else
        m.z = 0;

    if (strafe)
    {
        m.x += strafe * cosf(toRadians(yaw));
        m.y += strafe * sinf(toRadians(yaw));
    }
}

Vec3 getLookAtPoint(const Vec3 &from, const Vec3 &angle, float dist)
{
    Vec3 out;
    vecfromyawpitch(angle.x, angle.y, dist, 0, out);
    return from + out;
}

vector<Vec3> poss{};
void Visuals::Run()
{
    if (bEnabled && (bBox || bName || bLine || bHealthBar))
    {
        for (auto ent : *game::entities)
        {
            if (!ent)
                continue;

            if (ent == game::localPlayer)
                continue;

            if (ent->health <= 0)
                continue;

            bool sameTeam = !game::isEnemy(ent);
            if (!bTeam && sameTeam)
                continue;

            auto col = sameTeam ? color::GREEN : color::RED;
            if (strcmp(ent->team, "bots") == 0)
                col = color::YELLOW;
            Vec3 hitPos{};
            auto visible = game::isVisible(ent);
            if (!visible)
            {
                auto col2 = ImGui::ColorConvertU32ToFloat4(col);
                col2.x = min(col2.x * 0.5f, 1.0f);
                col2.y = min(col2.y * 0.5f, 1.0f);
                col2.z = min(col2.z * 0.5f, 1.0f);
                col = ImGui::ColorConvertFloat4ToU32(col2);
            }
            Vec3 head3d{ ent->o.x, ent->o.y, ent->o.z + 1.f };
            Vec3 feet3d{ ent->o.x, ent->o.y, ent->o.z - 14.f };
            Vec2 feet;
            Vec2 head;
            if (game::W2S(feet3d, feet) && game::W2S(head3d, head))
            {
                float dist = feet.dist(head);
                float width = .6f * dist;
                auto cw = width / 2;
                auto tl = Vec2{ head.x - cw, head.y };
                auto tr = Vec2{ head.x + cw, head.y };
                auto bl = Vec2{ feet.x - cw, feet.y };
                auto br = Vec2{ feet.x + cw, feet.y };
                // box, todo: 3d box
                if (bBox)
                {
                    draw::Rect(tl, tr, br, bl, col, 2.f);
                }

                // angle
                if (bAngle)
                {
                    Vec3 forward3d;
                    vecfromyawpitch(ent->angle.x, ent->angle.y, 10, 0, forward3d);
                    forward3d = ent->o + forward3d;
                    Vec2 f;
                    if (game::W2S(forward3d, f))
                    {
                        draw::Line(head, f, color::CYAN);
                    }
                }

                // line
                if (bLine)
                {
                    auto screen = game::getScreen();
                    draw::Line({ screen.x / 2, screen.y }, feet, col);
                }

                // health bar
                if (bHealthBar)
                {
                    float t = ent->health * 0.01f;
                    auto newTR = ImLerp(br, tr, t);
                    constexpr ImVec4 GREEN_COLOR(0.0f, 1.0f, 0.0f, 1.0f);
                    constexpr ImVec4 RED_COLOR(1.0f, 0.0f, 0.0f, 1.0f);
                    ImVec4 healthColor(ImLerp(RED_COLOR.x, GREEN_COLOR.x, t), ImLerp(RED_COLOR.y, GREEN_COLOR.y, t),
                                       ImLerp(RED_COLOR.z, GREEN_COLOR.z, t), ImLerp(RED_COLOR.w, GREEN_COLOR.w, t));
                    draw::Line(br, newTR, ImColor(healthColor), 2.5f);
                }

                // name
                if (bName)
                {
                    const char *text = ent->name;
                    auto nameSize = ImGui::CalcTextSize(text);
                    static ImGuiIO &io = ImGui::GetIO();
                    static auto bold = io.Fonts->Fonts[1];
                    ImGui::PushFont(bold);
                    draw::Text({ tl.x - (nameSize.x / 2), tl.y + (nameSize.y / 2) }, text, col, true);
                    ImGui::PopFont();
                }
            }
        }
    }
}
void Visuals::Menu()
{
    ImGui::Checkbox("Enable", &bEnabled);
    ImGui::Checkbox("Box", &bBox);
    ImGui::Checkbox("Name", &bName);
    ImGui::Checkbox("Line", &bLine);
    ImGui::Checkbox("Health Bar", &bHealthBar);
    ImGui::Checkbox("Team", &bTeam);
}