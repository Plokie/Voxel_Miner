#include "Engine.h"

Engine* Engine::_Instance;

void Engine::Init(_In_ HINSTANCE hInstance) {
	if (_Instance != nullptr) delete _Instance;
	_Instance = this;

	if(!winMgr.Init(hInstance)) {
		exit(991);
	}

	gfx = new Graphics();
	if(!gfx->Init(winMgr.window, winMgr.width, winMgr.height)) {
		exit(992);
	}

	Input::Init(winMgr.window);
	
	Resources::Init(gfx->GetDevice(), gfx->GetDeviceCtx(), gfx->GetLayout(), LAYOUTSIZE);

	
}

void Engine::Render(float dTime) {
	gfx->Render(sceneObjects);

}

void Engine::Update(float dTime) {
	for (pair<string, Object3D*> pair : sceneObjects) {
		pair.second->Update(dTime);
	}

	// Keep at end
	Input::EndUpdate();
}

//void Engine::OnResizeWindow(int width, int height)
//{
//	gfx.SetResolution(width, height);
//	//gfx.InitResolution(winMgr.window);
//}

Object3D* Engine::CreateObject3D(Object3D* obj, string name) {
	//sceneObjects[name] = new Object3D(gfx->GetDevice());
	//sceneObjects[name] = obj->models.push_back()
	sceneObjects[name] = obj;
	sceneObjects[name]->Start();

	return sceneObjects[name];
}

Object3D* Engine::CreateObject3D(Object3D* obj, string name, string meshName)
{
	//sceneObjects[name] = new Object3D(gfx->GetDevice());
	sceneObjects[name] = obj;
	sceneObjects[name]->AddModel(gfx->GetDevice());
	sceneObjects[name]->models[0]->SetMesh(meshName);
	sceneObjects[name]->Start();

	return sceneObjects[name];
}

Object3D* Engine::CreateObject3D(Object3D* obj, string name, string meshName, string texName)
{
	sceneObjects[name] = obj;
	sceneObjects[name]->AddModel(gfx->GetDevice());
	sceneObjects[name]->models[0]->SetMesh(meshName);
	sceneObjects[name]->models[0]->SetTexture(0, texName);
	sceneObjects[name]->Start();

	return sceneObjects[name];
}

bool Engine::Service() {
	return winMgr.StartWhile();
}

Engine::~Engine() {
	for(pair<string, Object3D*> pair : sceneObjects) {
		delete pair.second;
	}

	gfx->~Graphics();
	delete gfx;
	gfx = nullptr;
}