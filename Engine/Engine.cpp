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

	sceneObjects["test"] = new Object3D(gfx.GetDevice());
}

void Engine::Render(float dTime) {
	gfx.Render(sceneObjects);

}

void Engine::Update(float dTime) {
	float camSpeed = 5.f * dTime;

	if(Input::IsKeyHeld('W')) {
		//gfx.camera.AdjustPosition(gfx.camera.GetForwardVector() * camSpeed);
		gfx.camera.transform.position += gfx.camera.transform.forward() * camSpeed;
	}
	if(Input::IsKeyHeld('S')) {
		gfx.camera.transform.position += gfx.camera.transform.back() * camSpeed;
	}
	if(Input::IsKeyHeld('A')) {
		gfx.camera.transform.position += gfx.camera.transform.left() * camSpeed;
	}
	if(Input::IsKeyHeld('D')) {
		gfx.camera.transform.position += gfx.camera.transform.right() * camSpeed;
	}
	if(Input::IsKeyHeld(VK_SPACE)) {
		//gfx.camera.AdjustPosition(0.0f, camSpeed, 0.0f);
		gfx.camera.transform.position += Vector3(0, camSpeed, 0);
	}
	if(Input::IsKeyHeld(VK_SHIFT)) {
		//gfx.camera.AdjustPosition(0.0f, -camSpeed, 0.0f);
		gfx.camera.transform.position -= Vector3(0, camSpeed, 0);
	}

	XMFLOAT2 mouseDelta = Input::MouseDelta();
	float lookSpeed = 5.f * dTime;

	//gfx.camera.AdjustRotation(mouseDelta.y * lookSpeed, mouseDelta.x * lookSpeed, 0.0f);
	gfx.camera.transform.rotation += Vector3(mouseDelta.y * lookSpeed, mouseDelta.x * lookSpeed, 0.f);

	if(Input::IsKeyPressed(VK_ESCAPE)) {
		Input::SetMouseLocked(!Input::GetMouseLocked());
		gfx.camera.transform.rotation = Vector3::Zero();
	}


	sceneObjects["test"]->transform.rotation.y += 5.f * dTime;

	// Keep at end
	Input::EndUpdate();
}

bool Engine::Service() {
	return winMgr.StartWhile();
}

Engine::~Engine() {
	for(pair<string, Object3D*> pair : sceneObjects) {
		delete pair.second;
	}
}