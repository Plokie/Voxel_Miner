#include "Engine.h"

#include <assert.h>

#include "../Audio/Audio.h"

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

	audio = new Audio();

	Input::Init(winMgr.window);
	
	Resources::Init(gfx->GetDevice(), gfx->GetDeviceCtx(), gfx->GetLayout(), LAYOUTSIZE);

	
}

void Engine::Render(float dTime) {
	gfx->Render(currentScene);
}

void Engine::DestroyQueuedObjects() {
	// Safe to delete objects here, presumably no more operations

	// destroy max 30 per frame
	AcquireSRWLockExclusive(&gDestroyObjectsMutex);
	//int maxDestroy = 20;
	for(int i = 0; i < min(30, destroyObjectQueue.size()); i++) {
		DestroyObject3DImmediate(destroyObjectQueue[destroyObjectQueue.size()-1]);
		destroyObjectQueue.pop_back();
	}
	/*int destructions = 30;
	for(auto it = destroyObjectQueue.rbegin(); it != destroyObjectQueue.rend(); it++) {
		if(destructions <= 0) break;
		destroyObjectQueue.erase(it);
		destructions--;
	}*/
	
	/*for(const string& name : destroyObjectQueue) {
		DestroyObject3DImmediate(name);
	}*/
	ReleaseSRWLockExclusive(&gDestroyObjectsMutex);

	//destroyObjectQueue.clear();
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
	totalElapsedTime += dTime;

	for (pair<string, Object3D*> pair : *currentScene->GetSceneObjects3D()) {
		if(pair.second == nullptr) continue;

		if(!pair.second->hasRanStartFunction) {
			pair.second->Start();
			pair.second->hasRanStartFunction = true;
		}

		//AcquireSRWLockExclusive(&pair.second->gAccessMutex);
		pair.second->Update(dTime);
		//ReleaseSRWLockExclusive(&pair.second->gAccessMutex);
	}

	// Keep at end
	Input::EndUpdate();
	audio->Update();
	
}

const float& Engine::GetTotalElapsedTime()
{
	return this->totalElapsedTime;
}

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
	//assert(false);
	return false;
}



bool Engine::DestroyObject3DImmediate(string name)
{
	//todo: move procedure to scene class
	if(currentScene->GetSceneObjects3D()->count(name)) {
		Object3D*& objectToDelete = currentScene->GetSceneObjects3D()->at(name);
		objectToDelete->pendingDeletion = true;
		// Immediately acquire and release to hopefully make sure we dont destroy it while another thread is using it
		AcquireSRWLockExclusive(&objectToDelete->gAccessMutex);
		ReleaseSRWLockExclusive(&objectToDelete->gAccessMutex);
		
		delete objectToDelete;
		objectToDelete = nullptr;

		//currentScene->GetSceneObjects3D()->at(name) = nullptr;
		//ReleaseSRWLockShared(&objectToDelete->gAccessMutex);
		currentScene->GetSceneObjects3D()->erase(name);

		return true;
	}
	//assert(false);
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
	

	for(pair<const string, Scene*>& pair : scenes) {
		delete pair.second;
		pair.second = nullptr;
	}

	gfx->~Graphics();
	delete gfx;
	gfx = nullptr;
}