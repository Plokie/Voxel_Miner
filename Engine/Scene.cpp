#include "Scene.h"

#include "../Graphics/Graphics.h"

void Scene::Init(Graphics* gfx)
{
	this->gfx = gfx;
}

Object3D* Scene::CreateObject3D(Object3D* object3D, const string& name)
{
	sceneObjects3D[name] = object3D;

	InitializeSRWLock(&object3D->gAccessMutex);
	
	return object3D;
}

Object3D* Scene::CreateObject3D(Object3D* object3D, const string& name, const string& meshName)
{
	sceneObjects3D[name] = object3D;
	object3D->AddModel(gfx->GetDevice());
	object3D->models[0]->SetMesh(meshName);

	InitializeSRWLock(&object3D->gAccessMutex);

	return object3D;
}

Object3D* Scene::CreateObject3D(Object3D* object3D, const string& name, const string& meshName, const string& texName)
{
	sceneObjects3D[name] = object3D;
	object3D->AddModel(gfx->GetDevice());
	object3D->models[0]->SetMesh(meshName);
	object3D->models[0]->SetTexture(0, texName);

	InitializeSRWLock(&object3D->gAccessMutex);

	return object3D;
}

Object2D* Scene::CreateObject2D(Object2D* object2D, const string& name)
{
	sceneObjects2D[name] = object2D;
	
	object2D->Init(gfx->GetDevice());
	object2D->InitSelf(); //overriden fun to init child-specific features

	return object2D;
}

Object3D* Scene::GetObject3D(string name)
{
	return sceneObjects3D[name];
}

Object2D* Scene::GetObject2D(string name)
{
	return sceneObjects2D[name];
}

map<string, Object3D*>* Scene::GetSceneObjects3D()
{
	return &sceneObjects3D;
}

map<string, Object2D*>* Scene::GetSceneObjects2D()
{
	return &sceneObjects2D;
}

Scene::~Scene()
{
	for(pair<string, Object3D*> pair : sceneObjects3D) {
		delete pair.second;
	}
}
