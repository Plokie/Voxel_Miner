#pragma once

#include <string>
#include "Object3D.h"
#include "Object2D.h"
//#include "../Graphics/Graphics.h"

using namespace std;

class Graphics;
class Engine;

//todo: scene serialzation to disc. So its not always in memory
class Scene {
private:
	Graphics* gfx = nullptr;
	Engine* engine = nullptr;
	map<string, Object3D*> sceneObjects3D = {};
	map<string, Object2D*> sceneObjects2D = {};

public:
	XMFLOAT4 clearColor = { 145.f / 255.f, 217.f / 255.f, 1.0f, 1.0f };

	void Init(Graphics* gfx, Engine* engine);

	Scene(Graphics* pGfx): gfx(pGfx){}

	std::mutex createObjectMutex = {};

	Object3D* CreateObject3D(Object3D* object3D, const string& name);
	Object3D* CreateObject3D(Object3D* object3D, const string& name, const string& meshName);
	Object3D* CreateObject3D(Object3D* object3D, const string& name, const string& meshName, const string& texName);
	
	Object2D* CreateObject2D(Object2D* object2D, const string& name);
	
	Object3D* GetObject3D(string name);
	Object2D* GetObject2D(string name);

	template <typename T>
	T* GetObject3D(string name) {
		if(sceneObjects3D.count(name))
			return static_cast<T*>(sceneObjects3D[name]);
		else return nullptr;
	}

	template <typename T>
	T* GetObject2D(string name) {
		if(sceneObjects2D.count(name))
			return static_cast<T*>(sceneObjects2D[name]);
		else return nullptr;
	}

	map<string, Object3D*>* GetSceneObjects3D();
	map<string, Object2D*>* GetSceneObjects2D();

	~Scene();
};