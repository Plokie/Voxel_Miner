#include "GameStart.h"

#include "../Audio/Audio.h"


// Include all Object3D and child type references
// ---------------------------------------
#include "ExampleObject3D.h"
#include "TitleScreen.h"
#include "PlayerController.h"
#include "ChunkManager.h"

#include "../Engine/UI/Label.h"
#include "../Engine/UI/UIRect.h"
#include "../Engine/UI/Button.h"
#include "../Engine/UI/HorizontalLayoutRect.h"
#include "InventoryUI.h"
#include "Inventory.h"
// ---------------------------------------


// Call game-related initialisation stuff from here
void GameStart(Engine* engine) {

	// todo: move resource loading to a separate thread, so the main thread can display a loading screen
	Resources::LoadTexture(L"Data\\Textures\\img.dds", "head");
	Resources::LoadTexture(L"Data\\Textures\\pfp.dds", "pfp");
	Resources::LoadTexture(L"Data\\Textures\\block-select.dds", "block-select");
	Resources::LoadTexture(L"Data\\Textures\\chunkborder.dds", "chunkborder");
	//Resources::LoadTexture(L"Data\\Textures\\grass2.dds", "grass");
	Resources::LoadTexture(L"Data\\Textures\\atlas.dds", "atlas");
	Resources::LoadTexture(L"Data\\Textures\\item-atlas.dds", "item-atlas");

	Resources::LoadPixelShader(L"pixelshader.cso", "pixelshader");
	Resources::LoadPixelShader(L"pixelshellgrass.cso", "pixelshellgrass");
	//Resources::LoadPixelShader(L"demopixelshader.cso", "demopshader");

	Resources::LoadVertexShader(L"vertexshader.cso", "vertexshader");
	Resources::LoadVertexShader(L"watervertexshader.cso", "watervertexshader");
	Resources::LoadVertexShader(L"vertexshellgrass.cso", "vertexshellgrass");

	Audio::LoadClipStream("Data\\Music\\UndercoverVampirePoliceman.ogg", "MusicBg");
	Audio::LoadClip("Data\\Sounds\\hit.wav", "hit");

	Audio::Play("MusicBg", 1.f);

	Resources::LoadMesh("cube");
	Resources::LoadMesh(exampleFloorVertices, ARRAYSIZE(exampleFloorVertices), exampleCubeIndices, ARRAYSIZE(exampleCubeIndices), "floorMesh");
	Resources::LoadMesh(chunkBorderVertices, ARRAYSIZE(chunkBorderVertices), exampleCubeIndices, ARRAYSIZE(exampleCubeIndices), "chunkborder");
	Resources::LoadMesh(exampleInverseCubeVertices, ARRAYSIZE(exampleInverseCubeVertices), exampleInverseCubeIndices, ARRAYSIZE(exampleInverseCubeIndices), "inverse-cube");

	//todo: scene serialzation to disc. So its not always in memory

	TitleScreen::Setup(engine);

	Scene* gameScene = new Scene(Graphics::Get());

	gameScene->CreateObject3D(new ChunkManager(), "AChunkManager");
	gameScene->CreateObject3D(new PlayerController(), "PlayerController");
	gameScene->CreateObject3D(new Inventory(), "Inventory");
	//gameScene->CreateObject3D(ChunkManager::Create(&Graphics::Get()->camera.transform), "ChunkManager");
	gameScene->GetObject3D("PlayerController")->transform.position = Vector3(0, 10, 0);

	gameScene->CreateObject2D(new Label(L"Data\\Fonts\\Baloo2.spritefont", XMFLOAT4(0, 0, 0, 1.0f)), "fps-counter");
	gameScene->GetObject2D("fps-counter")->SetPosition(Vector2(0.f, 80.f));
	gameScene->GetObject2D<Label>("fps-counter")->SetColour(1.0f, 1.0f, 1.0f, 1.0f);

	gameScene->CreateObject2D(new Label(L"Data\\Fonts\\Baloo2.spritefont", XMFLOAT4(0, 0, 0, 1.0f)), "worldpos");
	gameScene->GetObject2D("worldpos")->SetPosition(Vector2(0.f, 104.f));
	gameScene->GetObject2D<Label>("worldpos")->SetColour(1.0f, 1.0f, 1.0f, 1.0f);

	gameScene->CreateObject2D(new InventoryUI(engine, gameScene), "invUI");

	//Setup(engine, gameScene);

	Audio::SetListener(&Graphics::Get()->camera.transform);

	
	engine->AddScene(gameScene, "game");

}

// Ideally don't want to use this, but it's here as an option
// Instead rely on the Object3D.Update(dTime) call for game logic updates
// 
// Create a new class inheriting from Object3D and override Update(float dTime)
// 
void GameUpdate(Engine* engine, float dTime) {
}