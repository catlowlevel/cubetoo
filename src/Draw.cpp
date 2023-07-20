#include "Draw.h"

inline ImVec2 Vec2ToImVec2(const Vec2& vec)
{
    return ImVec2{ vec.x, vec.y };
}

namespace draw
{
    void Line(const Vec2& src, const Vec2& dst, ImU32 color, float thickness)
    {
        Line(Vec2ToImVec2(src), Vec2ToImVec2(dst), color, thickness);
    }
    void Line(const ImVec2& src, const ImVec2& dst, ImU32 color, float thickness)
    {
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        drawList->AddLine(src, dst, color, thickness);
    }
    void Rect(const Vec2& p1, const Vec2& p2, ImU32 color, float thickness)
    {
        static ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        drawList->AddRect(Vec2ToImVec2(p1), Vec2ToImVec2(p2), color);
    }
    void Rect(const Vec2& p1, const Vec2& p2, const Vec2& p3, const Vec2& p4, ImU32 color, float thickness)
    {
        Line(p1, p2, color, thickness);
        Line(p2, p3, color, thickness);
        Line(p3, p4, color, thickness);
        Line(p4, p1, color, thickness);
    }
    void Text(Vec2 position, const char* text, ImU32 color, bool center)
    {
        auto drawList = ImGui::GetBackgroundDrawList();
        if (center)
        {
            ImVec2 textSize = ImGui::CalcTextSize(text);  // Calculate the size of the text
            position = { position.x - (textSize.x / 2), position.y - (textSize.y / 2) };  // Calculate centered position
        }
        drawList->AddText({ position.x, position.y }, color, text);
    }

    void Circle(Vec2 center, float radius, ImU32 color, float thickness)
    {
        auto drawList = ImGui::GetBackgroundDrawList();
        drawList->AddCircle({ center.x, center.y }, radius, color, 0, thickness);
    }

    // void Rect(const  p){

    // }
};  // namespace draw
