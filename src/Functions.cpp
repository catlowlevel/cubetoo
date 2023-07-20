#include "Functions.h"

#include <Windows.h>
#include <imgui/imgui.h>

#include <iostream>

#include "Cheat/Aim.h"
#include "Cheat/Visuals.h"
#include "Game.h"
#include "Hooks.h"
#include "Ui.h"
#include "Util.h"

using namespace std;

void Funcs::Init()
{
#define ADD_CHEAT(instance) ui::menu::funcs.emplace_back(std::make_pair(#instance, std::make_unique<instance>()))
#define ADD_CHEAT_NAME(name, instance) ui::menu::funcs.emplace_back(std::make_pair(name, std::make_unique<instance>()))
    ADD_CHEAT(Visuals);
    ADD_CHEAT(Aim);

    // static void *SetRelMouseMove = (void *)GetProcAddress(GetModuleHandle(L"SDL2.dll"), "SDL_"
    //                                                                                     "SetRelativeMouseMode");
    // HM_CE(SetRelMouseMove, relMouse);
    cout << "intersectclosest -> " << hex << (void *)game::intersectclosest << endl;
    cout << "Functions Initialized!" << endl;
}

LRESULT CALLBACK hk_WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    if (ui::menu::isOpen() && ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;
    switch (message)
    {
        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_OEM_3:
                    ui::menu::Toggle();
                    return true;
                case VK_DELETE:
                case VK_END:
                    extern bool exiting;
                    exiting = true;

                    // FreeLibrary(ac->win.mod);
                    return true;
            }
            break;
        }
        case WM_MOUSEMOVE:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_MOUSEWHEEL:
        case WM_MOUSELEAVE:
        case WM_MOUSEHOVER:
        {
            if (ui::menu::isOpen())
                return true;
            break;
        }
        default:
            break;
    }

    static auto oWinProc = hookManager->GetHooker(WINDOWPROC_HASH)->GetOriginal<WNDPROC>();
    return CallWindowProc(oWinProc, hWnd, message, wParam, lParam);
}

void ui::Update()
{
    if (game::entities->size > 0)
        for (auto &[name, cheat] : menu::funcs)
            cheat->Run();
}

void ui::menu::Draw()
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

BOOL WINAPI hk_SwapBuffers(HDC hDc)
{
    game::InitOpenGL(hDc);
    ui::Begin();
    ui::Update();
    if (ui::menu::isOpen())
        ui::menu::Draw();
    ui::End();

    // //using wglSwapBuffers = BOOL(WINAPI*)(HDC);
    static auto oSwapBuffers = hookManager->GetHooker(SWAPBUFFERS_HASH)->GetOriginal<BOOL(WINAPI *)(HDC)>();
    return oSwapBuffers(hDc);
}