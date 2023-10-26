#include "Engine.h"

void Engine::Init(_In_ HINSTANCE hInstance) {

	if(!winMgr.Init(hInstance)) {
		exit(991);
	}

	if(!gfx.Init(winMgr.window, winMgr.width, winMgr.height)) {
		exit(992);
	}

	Input::Init(winMgr.window);
	
	Resources::Init(gfx.GetDevice(), gfx.GetDeviceCtx(), gfx.GetLayout(), LAYOUTSIZE);
	Resources::LoadTexture(L"Data\\Textures\\err.dds", "err");
	Resources::LoadTexture(L"Data\\Textures\\img.dds", "head");
	Resources::LoadTexture(L"Data\\Textures\\pfp.dds", "pfp");
	Resources::LoadTexture(L"Data\\Textures\\grass.dds", "grass");
	Resources::LoadPixelShader(L"pixelshader.cso", "pixelshader");
	Resources::LoadPixelShader(L"demopixelshader.cso", "demopshader");
	Resources::LoadVertexShader(L"vertexshader.cso", "vertexshader");

	Resources::LoadMesh("cube");
	Resources::LoadMesh(exampleFloorVertices, ARRAYSIZE(exampleFloorVertices), exampleCubeIndices, ARRAYSIZE(exampleCubeIndices), "floorMesh");

	sceneObjects["test1"] = new Object3D(gfx.GetDevice());
	sceneObjects["test2"] = new Object3D(gfx.GetDevice());
	sceneObjects["test3"] = new Object3D(gfx.GetDevice());
	sceneObjects["test4"] = new Object3D(gfx.GetDevice());
	sceneObjects["floor"] = new Object3D(gfx.GetDevice());

	sceneObjects["test1"]->models[0]->SetMesh("cube");
	sceneObjects["test2"]->models[0]->SetMesh("cube");
	sceneObjects["test3"]->models[0]->SetMesh("cube");
	sceneObjects["test4"]->models[0]->SetMesh("cube");
	sceneObjects["floor"]->models[0]->SetMesh("floorMesh");

	sceneObjects["test1"]->transform.position = Vector3(0.f, 0.f, 0.f);
	sceneObjects["test2"]->transform.position = Vector3(-8.f, 0.f, 0.f);
	sceneObjects["test3"]->transform.position = Vector3(8.f, 5.f, 0.f);
	sceneObjects["test4"]->transform.position = Vector3(0.f, 8.f, 0.f);

	sceneObjects["floor"]->transform.position = Vector3(0.f, -10.f, 0.f);

	sceneObjects["test2"]->transform.scale = Vector3(2.f, 2.f, 2.f);
	sceneObjects["floor"]->transform.scale = Vector3(100.f, 1.f, 100.f);

	sceneObjects["test3"]->models[0]->alpha = 0.5f;
	sceneObjects["test2"]->models[0]->alpha = 0.5f;

	sceneObjects["test1"]->models[0]->SetTexture(0, "grass");
	sceneObjects["test2"]->models[0]->SetTexture(0, "head");
	sceneObjects["test3"]->models[0]->SetTexture(0, "pfp");
	sceneObjects["test4"]->models[0]->SetTexture(0, "head");
	sceneObjects["floor"]->models[0]->SetTexture(0, "grass");

	sceneObjects["test4"]->models[0]->SetPixelShader(0, "demopshader");
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

	if(Input::IsPadButtonHeld(0, XINPUT_GAMEPAD_A)) {
		gfx.camera.transform.position += Vector3(0, camSpeed, 0);
	}

	XMFLOAT2 mouseDelta = Input::MouseDelta();
	float lookSpeed = 5.f * dTime;

	//gfx.camera.AdjustRotation(mouseDelta.y * lookSpeed, mouseDelta.x * lookSpeed, 0.0f);
	if(Input::GetMouseLocked())
		gfx.camera.transform.rotation += Vector3(mouseDelta.y * lookSpeed, mouseDelta.x * lookSpeed, 0.f);

	if(Input::IsKeyPressed(VK_ESCAPE)) {
		Input::SetMouseLocked(!Input::GetMouseLocked());
		gfx.camera.transform.rotation = Vector3::Zero();
	}

	if(Input::IsMouseKeyPressed(MOUSE_L)) {
		//MessageBox(0, L"L MOUSE PRESSED", 0, 0);
	}

	if(Input::IsKeyPressed('J')) {
		gfx.windowWidth = winMgr.width;
		gfx.windowHeight = winMgr.height;
		gfx.InitResolution(winMgr.window);
	}


	sceneObjects["test1"]->transform.rotation.y += 5.f * dTime;
	sceneObjects["test4"]->transform.rotation.x += -5.f * dTime;
	sceneObjects["test2"]->transform.rotation.x += -2.f * dTime;
	sceneObjects["test3"]->transform.rotation += Vector3(-2.f * dTime, 2.f * dTime, -2.f * dTime);

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