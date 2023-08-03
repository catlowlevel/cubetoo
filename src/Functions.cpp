#include "Functions.h"

#include <Windows.h>
#include <imgui/imgui.h>

#include <iostream>

#include "Cheat/Aim.h"
#include "Cheat/Visuals.h"
#include "Game.h"
#include "Hooks.h"
#include "Logger.h"
#include "Ui.h"

using namespace std;

using fn_wglSwapBuffers = BOOL(WINAPI *)(HDC);
using fn_wndproc = WNDPROC;
fn_wglSwapBuffers oSwapBuffers;
fn_wndproc oWinProc;

void Funcs::Init()
{
#define ADD_CHEAT(instance) ui::menu::funcs.emplace_back(std::make_pair(#instance, std::make_unique<instance>()))
#define ADD_CHEAT_NAME(name, instance) ui::menu::funcs.emplace_back(std::make_pair(name, std::make_unique<instance>()))
    ADD_CHEAT(Visuals);
    ADD_CHEAT(Aim);

    oSwapBuffers = hookManager->GetHooker(SWAPBUFFERS_HASH)->GetOriginal<fn_wglSwapBuffers>();
    oWinProc = hookManager->GetHooker(WINDOWPROC_HASH)->GetOriginal<fn_wndproc>();

    spdlog::info("Functions Initialized!");
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

    return CallWindowProc(oWinProc, hWnd, message, wParam, lParam);
}

BOOL WINAPI hk_SwapBuffers(HDC hDc)
{
    game::InitOpenGL(hDc);
    ui::Begin();
    ui::Update();
    if (ui::menu::isOpen())
        ui::menu::Draw();
    ui::End();

    return oSwapBuffers(hDc);
}