#include "Scene.h"

#include "../Graphics/Graphics.h"

void Scene::Init(Graphics* gfx)
{
	this->gfx = gfx;
}

Object3D* Scene::CreateObject3D(Object3D* object3D, const string& name)
{
	sceneObjects3D[name] = object3D;
	
	return object3D;
}

Object3D* Scene::CreateObject3D(Object3D* object3D, const string& name, const string& meshName)
{
	sceneObjects3D[name] = object3D;
	object3D->AddModel(gfx->GetDevice());
	object3D->models[0]->SetMesh(meshName);
	return object3D;
}

Object3D* Scene::CreateObject3D(Object3D* object3D, const string& name, const string& meshName, const string& texName)
{
	sceneObjects3D[name] = object3D;
	object3D->AddModel(gfx->GetDevice());
	object3D->models[0]->SetMesh(meshName);
	object3D->models[0]->SetTexture(0, texName);
	return object3D;
}

Object3D* Scene::GetObject3D(string name)
{
	return sceneObjects3D[name];
}

map<string, Object3D*>* Scene::GetSceneObjects3D()
{
	return &sceneObjects3D;
}

Scene::~Scene()
{
	for(pair<string, Object3D*> pair : sceneObjects3D) {
		delete pair.second;
	}
}
