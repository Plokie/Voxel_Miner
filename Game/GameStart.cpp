#include "GameStart.h"




// Include all Object3D and child type references
// ---------------------------------------
#include "ExampleObject3D.h"
#include "CameraController.h"
#include "ChunkManager.h"
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
	engine->CreateObject3D(new ExampleObject3D(-2.f, 0.f, 0.f), "test2", "cube", "head");

	
	engine->CreateObject3D(ChunkManager::Create(&Graphics::Get()->camera.transform), "ChunkManager");
	
	engine->sceneObjects["test2"]->transform.position = Vector3(-8.f, 0.f, 0.f);
}

// Ideally don't want to use this, but it's here as an option
// Instead rely on the Object3D.Update(dTime) call for game logic updates
// 
// Create a new class inheriting from Object3D and override Update(float dTime)
// 
void GameUpdate(Engine* engine, float dTime) {
}