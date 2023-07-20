#include "Ui.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl2.h>
#include <imgui/imgui_impl_win32.h>

#include <iostream>

#include "Game.h"
#include "Util.h"

namespace ui
{
    void Update()
    {
        if (game::entities->size > 0)
            for (auto &[name, cheat] : menu::funcs)
                cheat->Run();
    }

    void menu::Draw()
    {
        ToggleMouse(false);
        ImGui::SetNextWindowSize({ 300, 300 }, ImGuiCond_FirstUseEver);
        ImGui::Begin("TOOCUBE");

        if (ImGui::BeginTabBar("Cheats"))
        {
            for (auto &[name, cheat] : menu::funcs)
            {
                if (ImGui::BeginTabItem(name))
                {
                    cheat->Menu();
                    ImGui::EndTabItem();
                }
            }
            ImGui::Separator();
            ImGui::EndTabBar();
        }

        ImGui::End();
    }
    void Begin()
    {
        game::ogl.SwitchContext(OGLCTX::CREATED);
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }
    void End()
    {
        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
        game::ogl.SwitchContext(OGLCTX::ORIGINAL);
    }

    void ToggleMouse(bool enable)
    {
        static auto SetRelMouseMode = util::GetExportFunction<int (*)(int)>(L"SDL2.dll", "SDL_"
                                                                                         "SetRelativeMouseMo"
                                                                                         "de");
        SetRelMouseMode(enable);
    }
    bool bOpen = false;
    void menu::Toggle()
    {
        bOpen = !bOpen;
        ToggleMouse(!bOpen);
    }

    bool menu::isOpen()
    {
        return bOpen;
    }
    void menu::Init()
    {
        ImGuiIO &io = ImGui::GetIO();

        io.Fonts->Clear();
        io.Fonts->AddFontFromFileTTF("C:\\Windows\\fonts\\Hack Regular Nerd Font Complete Windows Compatible.ttf", 15);
        io.Fonts->AddFontFromFileTTF("C:\\Windows\\fonts\\Hack Bold Nerd Font Complete Windows Compatible.ttf", 15);
        io.Fonts->Build();

        // https://github.com/ocornut/imgui/issues/707#issuecomment-1494706165
        ImGuiStyle &style = ImGui::GetStyle();
        style.WindowRounding = 0.f;
        style.FrameRounding = 0.f;
        style.ScrollbarRounding = 0;

        style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 0.90f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.15f, 0.90f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.85f);
        style.Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.01f, 1.00f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.83f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.87f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.01f, 0.01f, 0.02f, 0.80f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.55f, 0.53f, 0.55f, 0.51f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.91f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.83f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 0.62f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 0.84f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.48f, 0.72f, 0.89f, 0.49f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.69f, 0.99f, 0.68f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.30f, 0.69f, 1.00f, 0.53f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.44f, 0.61f, 0.86f, 1.00f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.38f, 0.62f, 0.83f, 1.00f);
        style.Colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.70f, 0.60f, 0.60f, 1.00f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.90f, 0.70f, 0.70f, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
        style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
        style.Colors[ImGuiCol_Tab] = ImVec4(0.50f, 0.50f, 0.50f, 0.70f);
        style.Colors[ImGuiCol_TabHovered] = ImVec4(0.60f, 0.60f, 0.60f, 0.80f);
        style.Colors[ImGuiCol_TabActive] = ImVec4(0.50f, 0.50f, 0.50f, 0.90f);
        style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.40f, 0.40f, 0.40f, 0.70f);
        style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.40f, 0.40f, 0.40f, 0.80f);
    }
};  // namespace ui
