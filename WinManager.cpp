#include <Windows.h>
#include <iostream>
#include <cassert>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib)

#include "SimpleMath.h"
#include "WinManager.h"

LRESULT DefaultMsgHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		// WM_ACTIVATE is sent when the window is activated or deactivated.  
		// We pause the game when the window is deactivated and unpause it 
		// when it becomes active.  
	case WM_ACTIVATE:
		/*if(LOWORD(wParam) == WA_INACTIVE)
		{
			mWinData.appPaused = true;
		}
		else
		{
			mWinData.appPaused = false;
		}*/
		return 0;

		// WM_SIZE is sent when the user resizes the window.  
	case WM_SIZE:
		// width = LOWORD(lParam)
		// height = HIWORD(lParam)
		return 0;

		// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
	case WM_ENTERSIZEMOVE:
		/*mWinData.appPaused = true;
		mWinData.resizing = true;*/
		return 0;

		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
	case WM_EXITSIZEMOVE:
		/*mWinData.appPaused = false;
		mWinData.resizing = false;
		if(mpMyD3D)
			mpMyD3D->OnResize(mWinData.clientWidth, mWinData.clientHeight, *mpMyD3D);*/
		return 0;

		// WM_DESTROY is sent when the window is being destroyed.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

		// The WM_MENUCHAR message is sent when a menu is active and the user presses 
		// a key that does not correspond to any mnemonic or accelerator key. 
	case WM_MENUCHAR:
		// Don't beep when we alt-enter.
		return MAKELRESULT(0, MNC_CLOSE);

		// Catch this message so to prevent the window from becoming too small.
	case WM_GETMINMAXINFO:
		//((MINMAXINFO*)lParam)->ptMinTrackSize.x = mWinData.minClientWidth;
		//((MINMAXINFO*)lParam)->ptMinTrackSize.y = mWinData.minClientHeight;
		return 0;

	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

bool WinManager::Init(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	int w(1366), h(768);

//#if defined(DEBUG) | defined(_DEBUG)
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//#endif

	WNDCLASS wnd;
	wnd.style = CS_HREDRAW | CS_VREDRAW;
	wnd.lpfnWndProc = DefaultMsgHandler;
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hInstance = hInstance;
	wnd.hIcon = LoadIcon(0, IDI_APPLICATION);
	wnd.hCursor = LoadCursor(0, IDC_ARROW);
	wnd.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wnd.lpszMenuName = 0;
	wnd.lpszClassName = L"D3DWndClassName";

	if(!RegisterClass(&wnd)) {
		MessageBox(0, L"RegisterClass failed.", 0, 0);
		return false;
	}

	RECT R = { 0, 0, w, h };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);

	HWND mainWnd = CreateWindow(L"D3DWndClassName", L"DX11 Voxel Gen", WS_OVERLAPPEDWINDOW, 0, 0, w, h, 0, 0, hInstance, 0);


	ShowWindow(mainWnd, SW_SHOW);
	UpdateWindow(mainWnd);
}

bool WinManager::StartWhile() {
	MSG msg = { 0 };
	if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if(msg.message == WM_QUIT) {
			return false;
		}
	}

	return true;
}