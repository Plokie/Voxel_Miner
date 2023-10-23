#include <Windows.h>
#include <cassert>

#include "WinManager.h"
#include "Input.h"

LRESULT CALLBACK DefaultMsgHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	//case WM_CHAR:
	//	MessageBox(0, L"WM_CHAR", 0, 0);
	//	return 0;

	case WM_INPUT:
		Input::HandleRawInput((HRAWINPUT)lParam);
		return 0;

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
		//width = LOWORD(lParam)
		//height = HIWORD(lParam)
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

/// <summary>
/// Sets up a basic window
/// </summary>
/// <param name="hInstance"></param>
/// <param name="hPrevInstance"></param>
/// <param name="lpCmdLine"></param>
/// <param name="nCmdShow"></param>
/// <returns>True if window sucessfully created, false otherwise</returns>
bool WinManager::Init(_In_ HINSTANCE hInstance) {
	width = 1366;
	height = 768;

//#if defined(DEBUG) | defined(_DEBUG)
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//#endif

	// Window information setup
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = DefaultMsgHandler;
	//wnd.lpfnWndProc = DefWindowProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(0, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(0, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wndClass.lpszMenuName = 0;
	wndClass.lpszClassName = L"D3DWndClassName";

	if(!RegisterClass(&wndClass)) {
		MessageBox(0, L"RegisterClass failed.", 0, 0);
		return false;
	}
	
	// Create window rect and apply to current window rect
	RECT R = { 0, 0, width, height };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);

	// Create the window
	window = CreateWindow(
		L"D3DWndClassName",		//Window class name
		L"DX11 Voxel Gen",		//Window title
		WS_OVERLAPPEDWINDOW,	//Window style
		0, 0,					//Window X,Y position
		width, height,			//Window X,Y size
		0,						//Parent window
		0,						//Sub-menu windows (children)
		hInstance,				//Instance reference
		0						//Empty free param to pass to window
	);

	// Show window, and update the window information
	ShowWindow(window, SW_SHOW);
	SetForegroundWindow(window);
	SetFocus(window);
	UpdateWindow(window);

	return true;
}

WinManager::~WinManager() {
	if(window != NULL) {
		UnregisterClass(wndClass.lpszClassName, wndClass.hInstance);
		DestroyWindow(window);
	}
}

bool WinManager::StartWhile() {

	//Handle windows messages
	MSG msg = { 0 };
	while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if(msg.message == WM_QUIT) {
			return false;
		}
	}
	return true;
}