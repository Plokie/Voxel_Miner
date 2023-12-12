#pragma once

#include <vector>
#include <map>

#include "../Graphics/Graphics.h"
#include "WinManager.h"
#include "Input.h"
#include "Transform.h"
//#include "World.h";
#include "Object3D.h"
#include "..\Graphics\Resources.h"
#include "..\Graphics\ExampleMeshes.h"
#include "Scene.h"

class Audio;

using namespace std;

class Engine {
private:
	std::mutex gDestroyObjectsMutex;
	//SRWLOCK gCreateObjectsMutex;

	Graphics* gfx;
	Audio* audio;
	WinManager winMgr;
	//World world;
	static Engine* _Instance;
	vector<string> destroyObjectQueue = {};

	map<string, Scene*> scenes = {};
	Scene* currentScene = nullptr;

	//map<void*, bool> dbg_deletedObjects = {};

	float totalElapsedTime = 0.0f;

	bool isRunning = true;
public:
	
	//void OverwriteDeletedPtr(void* ptr);
	//bool IsObjDeleted(void* objPtr);

	// Initialise Engine
	void Init(_In_ HINSTANCE hInstance);

	void Render(float dTime);
	void Update(float dTime);

	const float& GetTotalElapsedTime();

	void DestroyQueuedObjects();

	Scene* GetCurrentScene();

	static void StopGame();

	static Engine* Get() { return _Instance; }

	Graphics* GetGraphics() { return gfx; }
	WinManager* GetWinManager() { return &winMgr; }

	Scene* AddScene(string name);
	Scene* AddScene(Scene* scene, string name);
	Scene* GetScene(string name);
	void SetScene(string name);

	/*template <typename T>
	T* CreateObject3D(string name) {
		sceneObjects[name] = new T();
		((Object3D*)sceneObjects[name])->Start();

		return sceneObjects[name];
	}*/


	//Object3D* CreateObject3D(Object3D* obj, string name);
	//Object3D* CreateObject3D(Object3D* obj, string name, string meshName);
	//Object3D* CreateObject3D(Object3D* obj, string name, string meshName, string texName);

	bool DestroyObject3D(string name);
	bool DestroyObject3D(Object3D* obj);

	bool DestroyObject3DImmediate(string name);
	bool DestroyObject3DImmediate(Object3D* obj);

	std::mutex* GetDestroyObjectsMutex();

	/// <summary>
	/// Call at start of main while loop loop
	/// </summary>
	/// <returns>if to continue running</returns>
	bool Service();

	~Engine();
};