#pragma once

#include <Windows.h>

class WinManager {
public:
	bool Init(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);
	//static LRESULT DefaultMsgHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	bool StartWhile();
};