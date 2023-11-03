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
	gfx->Render(sceneObjects);
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

void Engine::Update(float dTime) {
	for (pair<string, Object3D*> pair : sceneObjects) {
		if(pair.second == nullptr) continue;
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
	sceneObjects[name] = obj;
	
	obj->Start();

	return obj;
}

Object3D* Engine::CreateObject3D(Object3D* obj, string name, string meshName)
{
	//sceneObjects[name] = new Object3D(gfx->GetDevice());
	sceneObjects[name] = obj;
	obj->AddModel(gfx->GetDevice());
	obj->models[0]->SetMesh(meshName);
	obj->Start();

	return obj;
}

Object3D* Engine::CreateObject3D(Object3D* obj, string name, string meshName, string texName) {
	sceneObjects[name] = obj;
	obj->AddModel(gfx->GetDevice());
	obj->models[0]->SetMesh(meshName);
	obj->models[0]->SetTexture(0, texName);
	obj->Start();

	return obj;
}

bool Engine::DestroyObject3D(string name)
{
	destroyObjectQueue.push_back(name);
	return true;
}

bool Engine::DestroyObject3D(Object3D* obj)
{
	for(pair<string, Object3D*> pair : sceneObjects) {
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
	if(sceneObjects.count(name)) {
		delete sceneObjects[name];

		sceneObjects.erase(name);
		return true;
	}
	assert(false);
	return false;
}

bool Engine::DestroyObject3DImmediate(Object3D* obj)
{
	for(pair<string, Object3D*> pair : sceneObjects) {
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
	for(pair<string, Object3D*> pair : sceneObjects) {
		delete pair.second;
	}

	gfx->~Graphics();
	delete gfx;
	gfx = nullptr;
}