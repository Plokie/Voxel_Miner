#include "Engine.h"

void Engine::Init(_In_ HINSTANCE hInstance) {
	if(!winMgr.Init(hInstance)) {
		exit(991);
	}

	if(!gfx.Init(winMgr.window, winMgr.width, winMgr.height)) {
		exit(992);
	}
}

void Engine::Render() {
	gfx.Render();
}

bool Engine::Service() {
	return winMgr.StartWhile();
}