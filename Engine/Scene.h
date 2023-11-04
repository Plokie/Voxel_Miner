#pragma once

#include <string>
#include "Object3D.h"
//#include "../Graphics/Graphics.h"

using namespace std;

class Graphics;

class Scene {
private:
	Graphics* gfx;
	map<string, Object3D*> sceneObjects3D = {};
public:
	void Init(Graphics* gfx);

	Scene(Graphics* pGfx): gfx(pGfx){}

	Object3D* CreateObject3D(Object3D* object3D, const string& name);
	Object3D* CreateObject3D(Object3D* object3D, const string& name, const string& meshName);
	Object3D* CreateObject3D(Object3D* object3D, const string& name, const string& meshName, const string& texName);
	
	
	Object3D* GetObject3D(string name);

	map<string, Object3D*>* GetSceneObjects3D();

	~Scene();
};