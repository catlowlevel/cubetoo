#pragma once
#include <imgui/imgui.h>

#include "Vec.h"

// ChatGPT Generated, not tested
namespace color
{
    constexpr uint32_t WHITE = IM_COL32(255, 255, 255, 255);
    constexpr uint32_t BLACK = IM_COL32(0, 0, 0, 255);
    constexpr uint32_t RED = IM_COL32(255, 0, 0, 255);
    constexpr uint32_t GREEN = IM_COL32(0, 255, 0, 255);
    constexpr uint32_t BLUE = IM_COL32(0, 0, 255, 255);

    constexpr uint32_t YELLOW = IM_COL32(255, 255, 0, 255);
    constexpr uint32_t CYAN = IM_COL32(0, 255, 255, 255);
    constexpr uint32_t MAGENTA = IM_COL32(255, 0, 255, 255);
    constexpr uint32_t GRAY = IM_COL32(128, 128, 128, 255);
    constexpr uint32_t ORANGE = IM_COL32(255, 165, 0, 255);
    constexpr uint32_t PURPLE = IM_COL32(128, 0, 128, 255);
    constexpr uint32_t BROWN = IM_COL32(165, 42, 42, 255);
    constexpr uint32_t PINK = IM_COL32(255, 192, 203, 255);
    constexpr uint32_t NAVY = IM_COL32(0, 0, 128, 255);
    constexpr uint32_t TEAL = IM_COL32(0, 128, 128, 255);
    constexpr uint32_t LIME = IM_COL32(0, 255, 0, 255);

    constexpr uint32_t SILVER = IM_COL32(192, 192, 192, 255);
    constexpr uint32_t MAROON = IM_COL32(128, 0, 0, 255);
    constexpr uint32_t OLIVE = IM_COL32(128, 128, 0, 255);
    constexpr uint32_t DARK_GREEN = IM_COL32(0, 100, 0, 255);
    constexpr uint32_t TEAL_GREEN = IM_COL32(0, 128, 128, 255);
    constexpr uint32_t SKY_BLUE = IM_COL32(135, 206, 235, 255);
    constexpr uint32_t GOLD = IM_COL32(255, 215, 0, 255);
    constexpr uint32_t DARK_ORANGE = IM_COL32(255, 140, 0, 255);
    constexpr uint32_t INDIGO = IM_COL32(75, 0, 130, 255);
    constexpr uint32_t TURQUOISE = IM_COL32(64, 224, 208, 255);
}  // namespace color

namespace draw
{
    void Line(const Vec2& src, const Vec2& dst, ImU32 color, float thickness = 1.f);
    void Line(const ImVec2& src, const ImVec2& dst, ImU32 color, float thickness = 1.f);
    void Rect(const Vec2& p1, const Vec2& p2, ImU32 color, float thickness = 1.f);
    void Rect(const Vec2& p1, const Vec2& p2, const Vec2& p3, const Vec2& p4, ImU32 color, float thickness = 1.f);
    void Text(Vec2 position, const char* text, ImU32 color, bool center = false);
    void Circle(Vec2 center, float radius, ImU32 color, float thickness = 1.f);
};  // namespace draw
