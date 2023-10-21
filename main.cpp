#include <Windows.h>
#include <iostream>
#include <cassert>
#include <vector>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"DirectXTK.lib")
#pragma comment(lib)

#include "SimpleMath.h"

#include "Engine.h"
#include "Timer.h"

using namespace std;
using namespace DirectX;

// Main entry point for windows
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	Engine engine;
	engine.Init(hInstance);

	Timer dTimer;
	float dTime = 0.001f;

	HRESULT hr = CoInitialize(NULL);
	if(FAILED(hr)) {
		return 999;
	}


	while(engine.Service()) {
		dTimer.stop();
		dTime = dTimer.get_elapsed<float>();
		dTimer.start();

		engine.Update(dTime);

		engine.Render(dTime);
	}

	return 0;
}