#pragma once

#include <Windows.h>

class WinManager {
	WNDCLASS wndClass;
public:
	HWND window;
	int width, height;

	bool Init(_In_ HINSTANCE hInstance);
	bool StartWhile();
	~WinManager();
};