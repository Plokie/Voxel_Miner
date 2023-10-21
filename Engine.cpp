#include "Engine.h"

void Engine::Init(_In_ HINSTANCE hInstance) {
	if(!winMgr.Init(hInstance)) {
		exit(-1);
	}

	if(!gfx.Init(winMgr.window, winMgr.width, winMgr.height)) {
		exit(-2);
	}
}

void Engine::Render() {
	gfx.TestRender();
}

bool Engine::Service() {
	return winMgr.StartWhile();
}