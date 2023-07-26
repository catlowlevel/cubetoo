#pragma once
#include <Windows.h>

#include <functional>
#include <vector>

#include "Structs.h"

struct sScreen
{
    int x, y;
};

struct TraceResult
{
    Vec3 end;
    bool collided;
};
enum class OGLCTX
{
    ORIGINAL,
    CREATED
};
struct OGL
{
    HGLRC CTX;
    HGLRC oCTX;
    HDC hDc;
    OGLCTX currentCtx = OGLCTX::ORIGINAL;
    void SwitchContext(OGLCTX ctx)
    {
        currentCtx = ctx;
        if (ctx == OGLCTX::ORIGINAL)
        {
            wglMakeCurrent(hDc, oCTX);
        }
        else if (ctx == OGLCTX::CREATED)
        {
            wglMakeCurrent(hDc, CTX);
        }
    }
};

enum FLAG : int32_t
{
    SKIP_SAMETEAM = 0x00000001,
    SKIP_NOT_VISIBLE = 0x00000010
};

using fn_intersectclosest = Entity*(__fastcall*)(const Vec3& from, const Vec3& to, Entity* at, float& bestdist);
//  double fromc, double arg8, double _A0, float a8);
using fn_raycubelos = bool(__fastcall*)(const Vec3& from, const Vec3& to, Vec3& hitpos);
using fn_intersect = bool(__fastcall*)(Entity* ent, const Vec3& from, const Vec3& to, float& dist);

namespace game
{
    extern OGL ogl;
    extern Entity* localPlayer;
    extern Vector<Entity*>* entities;
    extern fn_intersectclosest intersectclosest;
    extern fn_intersect intersect;
    extern fn_raycubelos raycubelos;

    void InitVars();
    void InitOpenGL(HDC hDc);

    // helper
    bool W2S(Vec3 worldPosition, Vec2& screenPosition);
    bool isVisible(Entity* ent);
    bool isEnemy(Entity* ent);
    Entity* getClosestEnt(int32_t skipFlags = 0, const std::function<void(Entity*)>& func = nullptr);
    Vec3 calcAngle(Vec3 v);
    uintptr_t ModuleOffset(uintptr_t offset, bool read = false);
    template <typename T>
    T ModuleOffset(uintptr_t offset, bool read = false)
    {
        return RCAST<T>(ModuleOffset(offset, read));
    }

    // getter
    Vec2 getScreen(bool center = false);
};  // namespace game
