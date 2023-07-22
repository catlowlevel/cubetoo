#include "Game.h"

#include <GL/GL.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl2.h>
#include <imgui/imgui_impl_win32.h>

#include <numbers>

#include "Draw.h"
#include "Functions.h"
#include "Hooks.h"
#include "Ui.h"
#include "Util.h"

#pragma comment(lib, "opengl32.lib")

constexpr auto OFFSET_SCREEN = 0x345468;
constexpr auto OFFSET_VIEWMATRIX = 0x32D040;
constexpr auto OFFSET_LOCALPLAYER = 0x2A5730;
constexpr auto OFFSET_ENTITIES = 0x346C90;
constexpr auto OFFSET_FN_INTERSECTCLOSEST = 0x1DB2A0;
constexpr auto OFFSET_FN_RAYCUBELOS = 0x1140E0;

namespace game
{

    uintptr_t baseModule;
    sScreen* screen;
    float* viewMatrix;
    Vector<Entity*>* entities;
    Entity* localPlayer;

    fn_intersectclosest intersectclosest;
    fn_raycubelos raycubelos;
    void InitVars()
    {
        baseModule = RCAST<uintptr_t>(GetModuleHandle(L"sauerbraten.exe"));
        screen = ModuleOffset<sScreen*>(OFFSET_SCREEN);
        viewMatrix = ModuleOffset<float*>(OFFSET_VIEWMATRIX);
        uintptr_t* entList = ModuleOffset<uintptr_t*>(OFFSET_ENTITIES);
        entities = RCAST<Vector<Entity*>*>(entList);
        localPlayer = ModuleOffset<Entity*>(OFFSET_LOCALPLAYER, true);
        intersectclosest = RCAST<fn_intersectclosest>(ModuleOffset(OFFSET_FN_INTERSECTCLOSEST));
        raycubelos = RCAST<fn_raycubelos>(ModuleOffset(OFFSET_FN_RAYCUBELOS));
    }

    uintptr_t ModuleOffset(uintptr_t offset, bool read)
    {
        uintptr_t result = baseModule + offset;
        if (read)
            result = *RCAST<uintptr_t*>(result);
        return result;
    }
    bool isVisible(Entity* ent)
    {
        Vec3 out;
        return raycubelos(localPlayer->pos, ent->pos, out);
    }

    bool W2S(Vec3 worldPosition, Vec2& screenPosition)
    {
        Vec4 clipCoords;
        clipCoords.x = worldPosition.x * viewMatrix[0] + worldPosition.y * viewMatrix[4] +
                       worldPosition.z * viewMatrix[8] + viewMatrix[12];
        clipCoords.y = worldPosition.x * viewMatrix[1] + worldPosition.y * viewMatrix[5] +
                       worldPosition.z * viewMatrix[9] + viewMatrix[13];
        clipCoords.z = worldPosition.x * viewMatrix[2] + worldPosition.y * viewMatrix[6] +
                       worldPosition.z * viewMatrix[10] + viewMatrix[14];
        clipCoords.w = worldPosition.x * viewMatrix[3] + worldPosition.y * viewMatrix[7] +
                       worldPosition.z * viewMatrix[11] + viewMatrix[15];

        if (clipCoords.w < 0.1f)
            return false;

        Vec3 NDC;
        NDC.x = clipCoords.x / clipCoords.w;
        NDC.y = clipCoords.y / clipCoords.w;
        NDC.z = clipCoords.z / clipCoords.w;

        float w = (float)screen->x;
        float h = (float)screen->y;
        screenPosition.x = (w / 2 * NDC.x) + (NDC.x + w / 2);
        screenPosition.y = -(h / 2 * NDC.y) + (NDC.y + h / 2);
        return true;
    }
    OGL ogl;
    void InitOpenGL(HDC hDc)
    {
        static bool initialized = false;
        if (initialized)
            return;
        HWND hWnd = WindowFromDC(hDc);
        HookManager::HookWindowProc(hWnd);
        ogl.hDc = hDc;
        ogl.oCTX = wglGetCurrentContext();
        ogl.CTX = wglCreateContext(hDc);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplOpenGL2_Init();
        ImGui_ImplWin32_Init(hWnd);

        Funcs::Init();
        ui::menu::Init();
        initialized = true;
    }

    Entity* getClosestEnt(int32_t skipFlags, const std::function<void(Entity*)>& func)
    {
        float minDistance = 999999.f;
        Entity* lastEnt = nullptr;
        for (auto ent : *entities)
        {
            if (!ent)
                continue;
            if (ent == localPlayer)
                continue;
            if (ent->health <= 0)
                continue;

            if (skipFlags & SKIP_SAMETEAM && !game::isEnemy(ent))
                continue;

            if (skipFlags & SKIP_NOT_VISIBLE && !game::isVisible(ent))
                continue;

            if (func)
                func(ent);

            float dist = localPlayer->pos.dist(ent->pos);
            if (dist < minDistance)
            {
                minDistance = dist;
                lastEnt = ent;
            }
        }
        return lastEnt;
    }
    bool isEnemy(Entity* ent)
    {
        //TODO: check if team gamemode
        return strcmp(ent->team, game::localPlayer->team) != 0;
    }

    Vec3 calcAngle(Vec3 v)
    {
        float dist = Vec2{ v.x, v.y }.length();
        return { std::atan2(v.y, v.x) * 180 / std::numbers::pi_v<float>,
                 std::atan2(v.z, dist) * 180 / std::numbers::pi_v<float>, 0 };
    }

    Vec2 getScreen(bool center)
    {
        if (center)
            return { (float)screen->x / 2, (float)screen->y / 2 };
        return { (float)screen->x, (float)screen->y };
    }
};  // namespace game
