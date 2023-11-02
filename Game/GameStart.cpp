#include "GameStart.h"




// Include all Object3D and child type references
// ---------------------------------------
#include "ExampleObject3D.h"
#include "CameraController.h"
#include "Chunk.h"
// ---------------------------------------


// Call game-related initialisation stuff from here
void GameStart(Engine* engine) {
	Resources::LoadTexture(L"Data\\Textures\\err.dds", "err");
	Resources::LoadTexture(L"Data\\Textures\\img.dds", "head");
	Resources::LoadTexture(L"Data\\Textures\\pfp.dds", "pfp");
	Resources::LoadTexture(L"Data\\Textures\\grass2.dds", "grass");
	Resources::LoadTexture(L"Data\\Textures\\atlas.dds", "atlas");
	Resources::LoadPixelShader(L"pixelshader.cso", "pixelshader");
	Resources::LoadPixelShader(L"demopixelshader.cso", "demopshader");
	Resources::LoadVertexShader(L"vertexshader.cso", "vertexshader");

	Resources::LoadMesh("cube");
	Resources::LoadMesh(exampleFloorVertices, ARRAYSIZE(exampleFloorVertices), exampleCubeIndices, ARRAYSIZE(exampleCubeIndices), "floorMesh");

	engine->CreateObject3D(new CameraController(), "CameraController");
	//engine->CreateObject3D(new ExampleObject3D(5.f, 0.f, 0.f), "test1", "cube", "grass");
	engine->CreateObject3D(new ExampleObject3D(-2.f, 0.f, 0.f), "test2", "cube", "head");
	//engine->CreateObject3D(new ExampleObject3D(-2.f, 2.f, -2.f), "test3", "cube", "pfp");
	//engine->CreateObject3D(new ExampleObject3D(0.f, 5.f, 0.f), "test4", "cube", "head");
	//engine->CreateObject3D(new Object3D(), "floor", "floorMesh", "grass");

	engine->CreateObject3D(new Chunk(), "TestChunk");
	
	engine->sceneObjects["test2"]->transform.position = Vector3(-8.f, 0.f, 0.f);
	//engine->sceneObjects["test3"]->transform.position = Vector3(8.f, 5.f, 0.f);
	//engine->sceneObjects["test4"]->transform.position = Vector3(0.f, 8.f, 0.f);

	//engine->sceneObjects["floor"]->transform.position = Vector3(0.f, -10.f, 0.f);

	//engine->sceneObjects["test2"]->transform.scale = Vector3(2.f, 2.f, 2.f);
	//engine->sceneObjects["floor"]->transform.scale = Vector3(100.f, 1.f, 100.f);

	//engine->sceneObjects["test2"]->models[0]->alpha = 0.5f;
	//engine->sceneObjects["test3"]->models[0]->alpha = 0.5f;

	//engine->sceneObjects["test4"]->models[0]->SetPixelShader(0, "demopshader");
}

// Ideally don't want to use this, but it's here as an option
// Instead rely on the Object3D.Update(dTime) call for game logic updates
// 
// Create a new class inheriting from Object3D and override Update(float dTime)
// 
void GameUpdate(Engine* engine, float dTime) {
}