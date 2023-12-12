#include "Scene.h"

#include "../Graphics/Graphics.h"
#include "Engine.h"

void Scene::Init(Graphics* gfx, Engine* engine)
{
	this->gfx = gfx;
	this->engine = engine;
}

Object3D* Scene::CreateObject3D(Object3D* object3D, const string& name)
{
	//unique_lock<mutex> lock(createObjectMutex);
	if (sceneObjects3D.find(name) != sceneObjects3D.end()) return object3D;

	sceneObjects3D[name] = object3D;

	//InitializeSRWLock(&object3D->gAccessMutex);
	//InitializeSRWLock(&object3D->modelsMutex);
	
	//if(engine)
	//engine->OverwriteDeletedPtr(object3D);

	return object3D;
}

Object3D* Scene::CreateObject3D(Object3D* object3D, const string& name, const string& meshName)
{
	sceneObjects3D[name] = object3D;
	object3D->AddModel(gfx->GetDevice());
	object3D->models[0]->SetMesh(meshName);

	//InitializeSRWLock(&object3D->gAccessMutex);
	//InitializeSRWLock(&object3D->modelsMutex);

	//if(engine)
	//engine->OverwriteDeletedPtr(object3D);

	return object3D;
}

Object3D* Scene::CreateObject3D(Object3D* object3D, const string& name, const string& meshName, const string& texName)
{
	sceneObjects3D[name] = object3D;
	object3D->AddModel(gfx->GetDevice());
	object3D->models[0]->SetMesh(meshName);
	object3D->models[0]->SetTexture(0, texName);

	//InitializeSRWLock(&object3D->gAccessMutex);
	//InitializeSRWLock(&object3D->modelsMutex);

	//if(engine)
	//engine->OverwriteDeletedPtr(object3D);
	

	return object3D;
}

Object2D* Scene::CreateObject2D(Object2D* object2D, const string& name)
{
	sceneObjects2D[name] = object2D;
	
	object2D->Init(gfx->GetDevice());
	object2D->InitSelf(); //overriden func to init child-specific features

	return object2D;
}

Object3D* Scene::GetObject3D(string name)
{
	if(sceneObjects3D.count(name))
		return sceneObjects3D[name];
	else return nullptr;
}

Object2D* Scene::GetObject2D(string name)
{
	if(sceneObjects2D.count(name))
		return sceneObjects2D[name];
	else return nullptr;
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
	printf("Begin Delete scene");
	for(pair<const string, Object3D*>& pair : sceneObjects3D) {
		printf("Delete %s", pair.first.c_str());
		delete pair.second;
		pair.second = nullptr;
	}
}
