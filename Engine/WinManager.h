#pragma once

#include <Windows.h>
#include "../Graphics/Graphics.h"

class WinManager {
	WNDCLASS wndClass;

	static WinManager* _Instance;
public:
	HWND window;
	//Graphics* gfx;
	int width, height;

	bool minimized = false;
	bool maximized = false;
	bool resizing = false;

	static WinManager* Get() {
		return _Instance;
	}

	//WNDPROC CALLBACK MsgHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	bool Init(_In_ HINSTANCE hInstance);
	bool StartWhile();
	~WinManager();
};