#pragma once

#include <Windows.h>

class WinManager {
	WNDCLASS wndClass;
public:
	HWND window;
	int width, height;

	//LRESULT CALLBACK DefaultMsgHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	bool Init(_In_ HINSTANCE hInstance);
	bool StartWhile();
	~WinManager();
};