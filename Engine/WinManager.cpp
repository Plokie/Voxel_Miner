#include <Windows.h>
#include <windowsx.h>
#include <cassert>

#include "WinManager.h"
#include "Input.h"

WinManager* WinManager::_Instance;

LRESULT CALLBACK DefaultMsgHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	WinManager* winMgr = WinManager::Get();
	Graphics* gfx = Graphics::Get();

	switch(msg)
	{
	case WM_INPUT:
		Input::HandleRawInput((HRAWINPUT)lParam);
		return DefWindowProc(hwnd, msg, wParam, lParam);

		// WM_ACTIVATE is sent when the window is activated or deactivated.  
	case WM_CHAR:
		Input::HandleCharInput(wParam);
		return DefWindowProc(hwnd, msg, wParam, lParam);

	case WM_ACTIVATE:
		//if(LOWORD(wParam) == WA_INACTIVE)
		return 0;
	case WM_MOUSEMOVE:
		//int xPos = GET_X_LPARAM(lParam);
		//int yPos = GET_Y_LPARAM(lParam);

		return 0;
	case WM_MOUSEWHEEL:
		Input::HandleMouseWheel(wParam);

		return 0;
		// WM_SIZE is sent when the user resizes the window.  
	case WM_SIZE:
		winMgr->width = LOWORD(lParam);
		winMgr->height = HIWORD(lParam);

		if (wParam == SIZE_MINIMIZED) {
			winMgr->minimized = true;
			winMgr->maximized = false;
		}
		else if (wParam == SIZE_MAXIMIZED) {
			winMgr->minimized = false;
			winMgr->maximized = true;
			gfx->OnResize(winMgr->window, winMgr->width, winMgr->height);
		}
		else if (wParam == SIZE_RESTORED) {
			if (winMgr->minimized) { // Coming from a minimized state
				winMgr->minimized = false;
				gfx->OnResize(winMgr->window, winMgr->width, winMgr->height);
			}
			else if (winMgr->maximized) { // Coming from a maximized state
				winMgr->maximized = false;
				gfx->OnResize(winMgr->window, winMgr->width, winMgr->height);
			}
			else if (winMgr->resizing) {

			}
			else {
				gfx->OnResize(winMgr->window, winMgr->width, winMgr->height);
			}
		}
		
		return 0;

		// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
	case WM_ENTERSIZEMOVE:
		winMgr->resizing = true;
		return 0;

		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
	case WM_EXITSIZEMOVE:
		winMgr->resizing = false;

		gfx->OnResize(winMgr->window, winMgr->width, winMgr->height);
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
	_Instance = this;

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
		L"Voxel Miner",		//Window title
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