#include "GameStart.h"




// Include all Object3D and child type references
// ---------------------------------------
#include "ExampleObject3D.h"
#include "CameraController.h"
#include "ChunkManager.h"
#include "TitleScreen.h"

#include "../Engine/Label.h"
// ---------------------------------------


// Call game-related initialisation stuff from here
void GameStart(Engine* engine) {
	Resources::LoadTexture(L"Data\\Textures\\err.dds", "err");
	Resources::LoadTexture(L"Data\\Textures\\img.dds", "head");
	Resources::LoadTexture(L"Data\\Textures\\pfp.dds", "pfp");
	Resources::LoadTexture(L"Data\\Textures\\block-select.dds", "block-select");
	Resources::LoadTexture(L"Data\\Textures\\chunkborder.dds", "chunkborder");
	Resources::LoadTexture(L"Data\\Textures\\grass2.dds", "grass");
	Resources::LoadTexture(L"Data\\Textures\\atlas.dds", "atlas");

	Resources::LoadPixelShader(L"pixelshader.cso", "pixelshader");

	Resources::LoadPixelShader(L"demopixelshader.cso", "demopshader");
	Resources::LoadVertexShader(L"vertexshader.cso", "vertexshader");
	Resources::LoadVertexShader(L"watervertexshader.cso", "watervertexshader");

	Resources::LoadMesh("cube");
	Resources::LoadMesh(exampleFloorVertices, ARRAYSIZE(exampleFloorVertices), exampleCubeIndices, ARRAYSIZE(exampleCubeIndices), "floorMesh");
	Resources::LoadMesh(chunkBorderVertices, ARRAYSIZE(chunkBorderVertices), exampleCubeIndices, ARRAYSIZE(exampleCubeIndices), "chunkborder");

	Scene* titleScene = new Scene(Graphics::Get());

	titleScene->CreateObject3D(new TitleScreen(), "titlescreen");
	titleScene->CreateObject3D(new ExampleObject3D(-2.f, 0.f, 0.f), "test", "cube", "head");
	titleScene->GetObject3D("test")->transform.position = Vector3(0.f, 0.f, 5.f);

	titleScene->CreateObject2D(new Label(L"Data\\Fonts\\algerian.spritefont", "Test 2!", XMFLOAT4(0,0,0,1.0f)), "start-label");

	Scene* gameScene = new Scene(Graphics::Get());

	gameScene->CreateObject3D(new CameraController(), "CameraController");
	gameScene->CreateObject3D(ChunkManager::Create(&Graphics::Get()->camera.transform), "ChunkManager");
	
	engine->AddScene(titleScene, "title");
	engine->AddScene(gameScene, "game");
	
	//engine->SetScene("game");

	//engine.s
	//engine->GetScene("game")->GetObject3D("test2")->transform.position = Vector3(-8.f, 0.f, 0.f);
}

// Ideally don't want to use this, but it's here as an option
// Instead rely on the Object3D.Update(dTime) call for game logic updates
// 
// Create a new class inheriting from Object3D and override Update(float dTime)
// 
void GameUpdate(Engine* engine, float dTime) {
}