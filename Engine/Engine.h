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

using namespace std;

class Engine {
private:
	SRWLOCK gDestroyObjectsMutex;
	//SRWLOCK gCreateObjectsMutex;

	Graphics* gfx;
	WinManager winMgr;
	//World world;
	static Engine* _Instance;
	vector<string> destroyObjectQueue = {};

public:
	map<string, Object3D*> sceneObjects = {};
	
	// Initialise Engine
	void Init(_In_ HINSTANCE hInstance);

	void Render(float dTime);
	void Update(float dTime);

	void DestroyQueuedObjects();

	static Engine* Get() { return _Instance; }

	Graphics* GetGraphics() { return gfx; }
	WinManager* GetWinManager() { return &winMgr; }

	/*template <typename T>
	T* CreateObject3D(string name) {
		sceneObjects[name] = new T();
		((Object3D*)sceneObjects[name])->Start();

		return sceneObjects[name];
	}*/

	Object3D* CreateObject3D(Object3D* obj, string name);
	Object3D* CreateObject3D(Object3D* obj, string name, string meshName);
	Object3D* CreateObject3D(Object3D* obj, string name, string meshName, string texName);

	bool DestroyObject3D(string name);
	bool DestroyObject3D(Object3D* obj);

	bool DestroyObject3DImmediate(string name);
	bool DestroyObject3DImmediate(Object3D* obj);

	SRWLOCK* GetDestroyObjectsMutex();
	//SRWLOCK* GetCreateObjectsMutex();
	//void OnResizeWindow(int width, int height);

	/// <summary>
	/// Call at start of main while loop loop
	/// </summary>
	/// <returns>if to continue running</returns>
	bool Service();

	~Engine();
};