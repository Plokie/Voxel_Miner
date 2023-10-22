#include "Engine.h"

void Engine::Init(_In_ HINSTANCE hInstance) {
	if(!winMgr.Init(hInstance)) {
		exit(991);
	}

	if(!gfx.Init(winMgr.window, winMgr.width, winMgr.height)) {
		exit(992);
	}

	//input.Init(winMgr.window);
	Input::Init(winMgr.window);
}

void Engine::Render(float dTime) {
	gfx.Render(dTime);

}

void Engine::Update(float dTime) {

	// Keep at end
	Input::EndUpdate();
}

bool Engine::Service() {
	return winMgr.StartWhile();
}