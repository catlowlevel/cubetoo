#pragma once
#include <Windows.h>
LRESULT CALLBACK hk_WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL WINAPI hk_SwapBuffers(HDC hDc);

namespace Funcs
{
    void Init();
}