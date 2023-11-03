#include <Windows.h>
#include <iostream>
#include <cassert>
#include <vector>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"DirectXTK.lib")
#pragma comment(lib)

#include "SimpleMath.h"

#include "Engine/Engine.h"
#include "Engine/Timer.h"
#include "Game/GameStart.h"

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

	GameStart(&engine);


	while(engine.Service()) {
		dTimer.stop();
		dTime = dTimer.get_elapsed<float>();
		dTimer.start();

		GameUpdate(&engine, dTime);
		engine.Update(dTime);

		engine.Render(dTime);

		engine.DestroyQueuedObjects();
	}

	return 0;
}