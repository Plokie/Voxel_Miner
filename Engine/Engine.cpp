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
	float camSpeed = 5.f * dTime;

	if(Input::IsKeyHeld('W')) {
		gfx.camera.AdjustPosition(gfx.camera.GetForwardVector() * camSpeed);
	}
	if(Input::IsKeyHeld('S')) {
		gfx.camera.AdjustPosition(gfx.camera.GetBackwardVector() * camSpeed);
	}
	if(Input::IsKeyHeld('A')) {
		gfx.camera.AdjustPosition(gfx.camera.GetLeftVector() * camSpeed);
	}
	if(Input::IsKeyHeld('D')) {
		gfx.camera.AdjustPosition(gfx.camera.GetRightVector() * camSpeed);
	}
	if(Input::IsKeyHeld(VK_SPACE)) {
		gfx.camera.AdjustPosition(0.0f, camSpeed, 0.0f);
	}
	if(Input::IsKeyHeld(VK_SHIFT)) {
		gfx.camera.AdjustPosition(0.0f, -camSpeed, 0.0f);
	}

	XMFLOAT2 mouseDelta = Input::MouseDelta();
	float lookSpeed = 5.f * dTime;

	gfx.camera.AdjustRotation(mouseDelta.y * lookSpeed, mouseDelta.x * lookSpeed, 0.0f);

	if(Input::IsKeyPressed(VK_ESCAPE)) {
		Input::SetMouseLocked(!Input::GetMouseLocked());
		gfx.camera.SetRotation(0.f, 0.f, 0.f);
	}

	// Keep at end
	Input::EndUpdate();
}

bool Engine::Service() {
	return winMgr.StartWhile();
}