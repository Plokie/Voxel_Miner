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
#include "../Engine/UI/ProgressBar.h"
#include "InventoryUI.h"
#include "Inventory.h"
#include "HeldItem.h"
#include "Meshes/HeldItemMesh.h"
#include "Meshes/DroppedItemMesh.h"
// ---------------------------------------


void LoadResources() {
	// todo: move resource loading to a separate thread, so the main thread can display a loading screen
	Resources::LoadTexture(L"Data\\Textures\\img.dds", "head");
	Resources::LoadTexture(L"Data\\Textures\\crosshair.dds", "crosshair");
	Resources::LoadTexture(L"Data\\Textures\\pfp.dds", "pfp");
	Resources::LoadTexture(L"Data\\Textures\\block-select.dds", "block-select");
	Resources::LoadTexture(L"Data\\Textures\\chunkborder.dds", "chunkborder");
	Resources::LoadTexture(L"Data\\Textures\\bar-split.dds", "bar-split");
	//Resources::LoadTexture(L"Data\\Textures\\grass2.dds", "grass");
	Resources::LoadTexture(L"Data\\Textures\\atlas.dds", "atlas");
	Resources::LoadTexture(L"Data\\Textures\\item-atlas.dds", "item-atlas");

	Resources::LoadFont(L"Data\\Fonts\\Baloo2.spritefont", "Baloo");

	Resources::LoadPixelShader(L"pixelshader.cso", "pixelshader");
	Resources::LoadPixelShader(L"pixelshellgrass.cso", "pixelshellgrass");
	//Resources::LoadPixelShader(L"demopixelshader.cso", "demopshader");

	Resources::LoadVertexShader(L"vertexshader.cso", "vertexshader");
	Resources::LoadVertexShader(L"watervertexshader.cso", "watervertexshader");
	Resources::LoadVertexShader(L"vertexshellgrass.cso", "vertexshellgrass");
	Resources::LoadVertexShader(L"vertexshaderleaves.cso", "vertexshaderleaves");

	Audio::LoadClipStream("Data\\Music\\UndercoverVampirePoliceman.ogg", "MusicBg");
	Audio::LoadClip("Data\\Sounds\\hit.wav", "hit");
	Audio::LoadClip("Data\\Sounds\\hurt.wav", "hurt");
	Audio::LoadClip("Data\\Sounds\\eat.wav", "eat");

	Audio::Play("MusicBg", 1.f);

	Resources::LoadMesh("cube");
	Resources::LoadMesh(exampleFloorVertices, ARRAYSIZE(exampleFloorVertices), exampleCubeIndices, ARRAYSIZE(exampleCubeIndices), "floorMesh");
	Resources::LoadMesh(chunkBorderVertices, ARRAYSIZE(chunkBorderVertices), exampleCubeIndices, ARRAYSIZE(exampleCubeIndices), "chunkborder");
	Resources::LoadMesh(exampleInverseCubeVertices, ARRAYSIZE(exampleInverseCubeVertices), exampleInverseCubeIndices, ARRAYSIZE(exampleInverseCubeIndices), "inverse-cube");

	Resources::LoadMesh(heldItemVertices, ARRAYSIZE(heldItemVertices), heldItemIndices, ARRAYSIZE(heldItemIndices), "heldItemMesh");
	Resources::LoadMesh(droppedItemVertices, ARRAYSIZE(droppedItemVertices), droppedItemIndices, ARRAYSIZE(droppedItemIndices), "droppedItemMesh");
}


// Call game-related initialisation stuff from here
void GameStart(Engine* engine) {

	LoadResources();

	//todo: better randomness. Maybe random singleton for World / Block / Entity randomness
	srand((unsigned)time(NULL));

	TitleScreen::Setup(engine);

	Scene* gameScene = new Scene(Graphics::Get());

	gameScene->CreateObject3D(new ChunkManager(), "AChunkManager");
	Inventory* inventory = (Inventory*)gameScene->CreateObject3D(new Inventory(), "Inventory");
	inventory->LoadDefaultItems();

	PlayerController* pc = (PlayerController*)gameScene->CreateObject3D(new PlayerController(), "PlayerController");
	HeldItem* heldItem = (HeldItem*)gameScene->CreateObject3D(new HeldItem(), "HeldItem");
	heldItem->transform.SetParent(&pc->transform);
	heldItem->transform.position = {.7f, -0.3f, 0.5f};

	gameScene->GetObject3D("PlayerController")->transform.position = Vector3(0, 10, 0);

	gameScene->CreateObject2D(new Label("Baloo", XMFLOAT4(0, 0, 0, 1.0f)), "fps-counter");
	gameScene->GetObject2D("fps-counter")->SetPosition(Vector2(0.f, 80.f));
	gameScene->GetObject2D<Label>("fps-counter")->SetColour(1.0f, 1.0f, 1.0f, 1.0f);

	gameScene->CreateObject2D(new Label("Baloo", XMFLOAT4(0, 0, 0, 1.0f)), "worldpos");
	gameScene->GetObject2D("worldpos")->SetPosition(Vector2(0.f, 104.f));
	gameScene->GetObject2D<Label>("worldpos")->SetColour(1.0f, 1.0f, 1.0f, 1.0f);

	Label* scoreLabel = (Label*)gameScene->CreateObject2D(new Label("Baloo", XMFLOAT4(0, 0, 0, 1.0f)), "score");
	scoreLabel->SetAnchor({ 1.f, 0.f });
	scoreLabel->SetPivot(1.f, 0.f);
	scoreLabel->SetPosition({ -20.f, 90.f });
	scoreLabel->SetText("Score: 0");
	scoreLabel->SetColour(1.0f, 1.0f, 1.0f, 1.0f);
	inventory->AddOnScoreChangeEvent([scoreLabel](int score) {
		scoreLabel->SetText("Score: " + to_string(score));
	});


	UIRect* crosshair = (UIRect*)gameScene->CreateObject2D(new UIRect("crosshair", {1.f,1.f,1.f,.7f}), "crosshair");
	crosshair->SetAnchor({ 0.5f, 0.5f });
	crosshair->SetPivot({ 0.5f, 0.5f });
	crosshair->SetDimensions({ 20.f, 20.f });

	gameScene->CreateObject2D(new InventoryUI(engine, gameScene), "invUI");

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