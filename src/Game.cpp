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
// #pragma comment(lib, "glu32.lib")

constexpr auto SCREEN_OFFSET = 0x345468;
constexpr auto VIEWMATRIX_OFFSET = 0x32D040;
constexpr auto LOCALPLAYER_OFFSET = 0x2A5730;
constexpr auto ENTITIES_OFSSET = 0x346C90;
constexpr auto FN_INTERSECTCLOSEST_OFFSET = 0x1DB2A0;
constexpr auto FN_RAYCUBELOS_OFFSET = 0x1140E0;
// constexpr auto ENTITIES_OFSSET = 0x346C80;
namespace game
{

    uintptr_t baseModule;
    sScreen* screen;
    float* viewMatrix;
    Vector<Entity*>* entities;
    Entity* localPlayer;

    // uintptr_t dwIntersectClosest;
    fn_intersectclosest intersectclosest;
    fn_raycubelos raycubelos;
    void InitVars()
    {
        baseModule = RCAST<uintptr_t>(GetModuleHandle(L"sauerbraten.exe"));
        screen = ModuleOffset<sScreen*>(SCREEN_OFFSET);
        viewMatrix = ModuleOffset<float*>(VIEWMATRIX_OFFSET);
        uintptr_t* entList = ModuleOffset<uintptr_t*>(ENTITIES_OFSSET);
        entities = RCAST<Vector<Entity*>*>(entList);
        localPlayer = ModuleOffset<Entity*>(LOCALPLAYER_OFFSET, true);
        // dwIntersectClosest = ModuleOffset(FN_INTERSECTCLOSEST_OFFSET);
        intersectclosest = RCAST<fn_intersectclosest>(ModuleOffset(FN_INTERSECTCLOSEST_OFFSET));
        raycubelos = RCAST<fn_raycubelos>(ModuleOffset(FN_RAYCUBELOS_OFFSET));
    }

    // Entity* intersectclosest(const Vec3& from, const Vec3& to, Entity* at, float& bestdist)
    // {
    //     static auto intersectclosest =
    //         RCAST<Entity*(__fastcall*)(const Vec3& from, const Vec3& to, Entity* at, float& bestdist)>(
    //             dwIntersectClosest);
    //     return intersectclosest(from, to, at, bestdist);
    //     // Entity* result{};
    //     // asm volatile("lea %[bestDist], %%r9;"
    //     //              "mov %[at], %%r8;"
    //     //              "lea %[to], %%rdx;"
    //     //              "lea %[from], %%rcx;"
    //     //              "movq %[fn],%%rax;"
    //     //              "callq *%%rax;"
    //     //              "movq %%rax,%[res]"
    //     //              : [bestDist] "=g"(bestdist), [res] "=r"(result)
    //     //              : [at] "g"(at), [to] "g"(to), [from] "g"(from), [fn] "g"(dwIntersectClosest)
    //     //              : "rax", "rcx", "rdx", "r8", "r9");
    //     // return result;
    // }
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

        // ogl.SwitchContext(OGLCTX::CREATED);
        // glMatrixMode(GL_PROJECTION);
        // glLoadIdentity();

        // GLint viewport[4];
        // glGetIntegerv(GL_VIEWPORT, viewport);

        // glOrtho(0, viewport[2], viewport[3], 0, 1, -1);
        // glMatrixMode(GL_MODELVIEW);
        // glLoadIdentity();
        // glClearColor(0, 0, 0, 1);
        // ogl.SwitchContext(OGLCTX::ORIGINAL);

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

            // if (visible && !ent->isVisible())
            //     continue;
            // if (noSameTeam && ent->sameTeam())
            //     continue;
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
