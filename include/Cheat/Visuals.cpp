#include "Visuals.h"

#include <imgui/imgui_internal.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
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
Vec3 addDistanceAlongAngle(const Vec3 &position, const Vec3 &angle, float distance)
{
    // Convert angles from degrees to radians
    auto radAngle = game::calcAngle(angle);
    // radAngle.x -= 90.f;
    // float angleInRadiansZ = angle.z * (numbers::pi_v<float> / 180.0f);

    // Calculate the displacement in each direction
    float deltaX = cos(radAngle.y) * cos(radAngle.x);
    float deltaY = -sin(radAngle.y);
    float deltaZ = sin(radAngle.y) * cos(radAngle.x);

    // Update the position
    Vec3 updatedPosition;
    updatedPosition.x = position.x + deltaX;
    updatedPosition.y = position.y + deltaY;
    updatedPosition.z = position.z + deltaZ;

    return updatedPosition;
}
// Function to get the forward position from the given position and angle
Vec3 getForwardPosition(const Vec3 &position, const Vec3 &angle, double distance)
{
    // Convert angle from degrees to radians
    double yaw = angle.y * M_PI / 180.0;
    double pitch = angle.x * M_PI / 180.0;
    // pitch += 90;
    // auto ang = game::calcAngle(angle);
    // ang.x -= 90.0;

    // Calculate the forward direction
    double xForward = distance * cos(yaw) * cos(pitch);
    double yForward = distance * sin(pitch);
    double zForward = distance * sin(yaw) * cos(pitch);

    // Calculate the new position
    Vec3 forwardPosition;
    forwardPosition.x = position.x + xForward;
    forwardPosition.y = position.y + yForward;
    forwardPosition.z = position.z + zForward;

    return forwardPosition;
}
Vec3 move_forward(const Vec3 &pos, const Vec3 &euler_angles, float distance)
{
    // Define the forward direction vector (assuming it's the negative Z-axis)
    glm::vec3 forward_vector(0.0f, 0.0f, 1.0f);

    // Rotate the forward vector based on the Euler angles
    // glm::vec3 rotated_forward = glm::rotateX(glm::rotateY(forward_vector, euler_angles.y), euler_angles.x);
    // Rotate the forward vector based on the Euler angles (in the order of yaw, pitch, roll)
    glm::vec3 rotated_forward =
        glm::rotateZ(glm::rotateY(glm::rotateX(forward_vector, euler_angles.x), euler_angles.y), euler_angles.z);

    // Scale the rotated forward vector by the desired distance
    glm::vec3 displacement = distance * rotated_forward;

    // Add the displacement to the current position to get the new position
    glm::vec3 new_position = glm::vec3(pos.x, pos.y, pos.z) + displacement;

    return Vec3{ new_position.x, new_position.y, new_position.y };
}

// Function to convert degrees to radians
constexpr float toRadians(float degrees)
{
    return degrees * M_PI / 180.0f;
}

// Function to get the look direction vector
Vec3 getLookDirection(Vec3 angle)
{
    float yaw = toRadians(angle.x + 90);
    float pitch = toRadians(angle.y);

    float cosPitch = cos(pitch);
    float sinPitch = sin(pitch);
    float cosYaw = cos(yaw);
    float sinYaw = sin(yaw);

    float x = cosPitch * cosYaw;
    float y = sinYaw;
    float z = cosPitch * sinPitch;

    return Vec3(x, y, z);
}

// Function to get the point you are looking at
Vec3 getLookAtPoint(Vec3 pos, Vec3 angle, float distance)
{
    Vec3 direction = getLookDirection(angle);
    return Vec3(pos.x + direction.x * distance, pos.y + direction.y * distance, pos.z + direction.z * distance);
}

vector<Vec3> poss{};

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
void Visuals::Run()
{
    // float dist = game::localPlayer->pos.dist(closest->pos);
    // auto dt = closest->pos - game::localPlayer->pos;
    // dt = dt * dist;
    // auto res = game::localPlayer->pos + dt;
    // auto res = getLookAtPoint(game::localPlayer->pos, game::localPlayer->angle, 2.f);
    Vec3 res;
    vecfromyawpitch(game::localPlayer->angle.x, game::localPlayer->angle.y, 1, 0, res);
    Vec2 out;
    if (game::W2S(game::localPlayer->pos + res, out))
    {
        draw::Text(out, "X", color::RED);
        draw::Line(game::getScreen(), out, color::GREEN);
    }

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
            Vec3 head3d{ ent->pos.x, ent->pos.y, ent->pos.z + 1.f };
            // ent->angle.x += 1.f;
            Vec3 forawrd3d = getLookAtPoint(head3d, ent->angle, 10.f);
            Vec3 feet3d{ ent->pos.x, ent->pos.y, ent->pos.z - 14.f };
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

                Vec2 f;
                if (game::W2S(forawrd3d, f))
                {
                    draw::Line(head, f, color::CYAN);
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

    for (auto &pos : poss)
    {
        Vec2 out;
        if (game::W2S(pos, out))
        {
            draw::Text(out, "X", color::CYAN);
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

    if (ImGui::Button("POS"))
    {
        poss.emplace_back(getLookAtPoint(game::localPlayer->pos, game::localPlayer->angle, 10.f));
    }

    if (ImGui::Button("BRRR"))
    {
        void *rip_ptr = __builtin_return_address(0);
        uintptr_t rip = (uintptr_t)rip_ptr;
        cout << hex << rip << endl;

        game::getClosestEnt(0,
                            [](Entity *ent)
                            {
                                float dist{};
                                Entity *res{};
                                if ((res = game::intersectclosest(game::localPlayer->pos, ent->pos, game::localPlayer,
                                                                  dist, 0, 0, 0, 0)))
                                {
                                    cout << res->name << endl;
                                }
                            });
    }
}