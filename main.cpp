#include <Windows.h>
#include <iostream>
#include <cassert>
#include <vector>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"DirectXTK.lib")
#pragma comment(lib)

#include "SimpleMath.h"

#include "Engine.h"

using namespace std;
using namespace DirectX;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	Engine engine;
	engine.Init(hInstance);
	
	HRESULT hr = CoInitialize(NULL);
	if(FAILED(hr)) {
		return 999;
	}

	while(engine.Service()) {

		engine.Render();
	}

	return 0;
}