#include "Engine.h"

#include <assert.h>

Engine* Engine::_Instance;

void Engine::Init(_In_ HINSTANCE hInstance) {
	if (_Instance != nullptr) delete _Instance;
	_Instance = this;

	InitializeSRWLock(&gDestroyObjectsMutex);
	//InitializeSRWLock(&gCreateObjectsMutex);

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
	gfx->Render(currentScene);
}

void Engine::DestroyQueuedObjects() {
	// Safe to delete objects here, presumably no more operations

	AcquireSRWLockExclusive(&gDestroyObjectsMutex);
	for(const string& name : destroyObjectQueue) {
		DestroyObject3DImmediate(name);
	}
	ReleaseSRWLockExclusive(&gDestroyObjectsMutex);

	destroyObjectQueue.clear();
}

Scene* Engine::GetCurrentScene()
{
	assert(currentScene);
	return currentScene;
}

Scene* Engine::AddScene(string name)
{
	Scene* newScene = new Scene(gfx);
	this->scenes[name] = newScene;

	if(currentScene == nullptr) currentScene = newScene;

	return newScene;
}

Scene* Engine::AddScene(Scene* scene, string name)
{
	this->scenes[name] = scene;
	scene->Init(gfx);
	if(currentScene == nullptr) currentScene = scene;
	return scene;
}

Scene* Engine::GetScene(string name)
{
	if(scenes.count(name)) return scenes[name];
	return nullptr;
}

void Engine::SetScene(string name)
{
	if(scenes.count(name))
		currentScene = scenes[name];
#ifdef _DEBUG
	else assert(false); // SCENE BY NAME DOES NOT EXIST
#endif
}

void Engine::Update(float dTime) {
	for (pair<string, Object3D*> pair : *currentScene->GetSceneObjects3D()) {
		if(pair.second == nullptr) continue;

		if(!pair.second->hasRanStartFunction) {
			pair.second->Start();
			pair.second->hasRanStartFunction = true;
		}

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
//
//Object3D* Engine::CreateObject3D(Object3D* obj, string name) {
//	//sceneObjects[name] = obj;
//
//	currentScene->CreateObject3D(obj, name);
//	
//	//obj->Start();
//
//	return obj;
//}
//
//Object3D* Engine::CreateObject3D(Object3D* obj, string name, string meshName)
//{
//	//sceneObjects[name] = new Object3D(gfx->GetDevice());
//	currentScene->CreateObject3D(obj, name, meshName);
//	//obj->AddModel(gfx->GetDevice());
//	//obj->models[0]->SetMesh(meshName);
//	//obj->Start();
//
//	return obj;
//}
//
//Object3D* Engine::CreateObject3D(Object3D* obj, string name, string meshName, string texName) {
//	currentScene->CreateObject3D(obj, name, meshName, texName);
//	//obj->AddModel(gfx->GetDevice());
//	//obj->models[0]->SetMesh(meshName);
//	//obj->models[0]->SetTexture(0, texName);
//	//obj->Start();
//
//	return obj;
//}

bool Engine::DestroyObject3D(string name)
{
	destroyObjectQueue.push_back(name);
	return true;
}

bool Engine::DestroyObject3D(Object3D* obj)
{
	//todo: move procedure to scene class
	for(pair<string, Object3D*> pair : *currentScene->GetSceneObjects3D()) {
		if(pair.second == obj) {
			DestroyObject3D(pair.first);
			return true;
		}
	}
	//MessageBox(0, L"Could not find obj", 0, 0);
	assert(false);
	return false;
}



bool Engine::DestroyObject3DImmediate(string name)
{
	//todo: move procedure to scene class
	if(currentScene->GetSceneObjects3D()->count(name)) {
		delete currentScene->GetSceneObjects3D()->at(name);
		currentScene->GetSceneObjects3D()->at(name) = nullptr;

		currentScene->GetSceneObjects3D()->erase(name);
		return true;
	}
	assert(false);
	return false;
}

bool Engine::DestroyObject3DImmediate(Object3D* obj)
{
	for(pair<string, Object3D*> pair : *currentScene->GetSceneObjects3D()) {
		if(pair.second == obj) {
			DestroyObject3DImmediate(pair.first);
			return true;
		}
	}
	return false;
}

SRWLOCK* Engine::GetDestroyObjectsMutex()
{
	return &this->gDestroyObjectsMutex;
}
//SRWLOCK* Engine::GetCreateObjectsMutex()
//{
//	return &this->gCreateObjectsMutex;
//}

bool Engine::Service() {
	return winMgr.StartWhile();
}

Engine::~Engine() {
	

	for(auto pair : scenes) {
		delete pair.second;
	}

	gfx->~Graphics();
	delete gfx;
	gfx = nullptr;
}