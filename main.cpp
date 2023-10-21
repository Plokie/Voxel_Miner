#include <Windows.h>
#include <iostream>
#include <cassert>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib)

#include "SimpleMath.h"
#include "WinManager.h"

using namespace std;
using namespace DirectX;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	WinManager winMgr;

	if(!winMgr.Init(hInstance, hPrevInstance, lpCmdLine, nCmdShow)) {
		return 0;
	}

	
	while(winMgr.StartWhile()) {

		


	}
}