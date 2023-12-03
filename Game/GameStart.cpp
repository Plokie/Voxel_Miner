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
	Scene* titleScene = new Scene(Graphics::Get());

	titleScene->CreateObject3D(new TitleScreen(), "titlescreen");
	titleScene->CreateObject3D(new ExampleObject3D(-2.f, 0.f, 0.f), "test", "cube", "head");
	titleScene->GetObject3D("test")->transform.position = Vector3(0.f, 0.f, 5.f);


	UIRect* rect = (UIRect*)titleScene->CreateObject2D(new UIRect(), "rect");
	rect->SetColour(.5f, .5f, .5f, 1.f);
	rect->SetPosition(Vector2(0.f, 0.f));
	rect->SetAnchor(Vector2(0.5f, 0.5f));
	rect->SetDimensions(Vector2(500.f, 100.f));
	rect->SetPivot(Vector2(250.f, 50.f));
	
	Label* label = (Label*)titleScene->CreateObject2D(new Label(L"Data\\Fonts\\algerian.spritefont", "Start", XMFLOAT4(0,0,0,1.0f)), "start-label");
	label->SetPosition(Vector2(0.f, 0.f));
	label->SetAnchor(Vector2(0.5f, 0.5f));
	//label->SetDimensions(Vector2(500.f, 30.f));
	//label->SetDimensions(Vector2(label->GetText().size() * 16.f, 30.f));
	
	label->SetPivot(label->GetDimensions() * 0.5f);
	label->SetParent(rect);

	Scene* gameScene = new Scene(Graphics::Get());

	gameScene->CreateObject3D(new ChunkManager(), "AChunkManager");
	gameScene->CreateObject3D(new PlayerController(), "PlayerController");
	//gameScene->CreateObject3D(ChunkManager::Create(&Graphics::Get()->camera.transform), "ChunkManager");
	gameScene->GetObject3D("PlayerController")->transform.position = Vector3(0, 10, 0);

	gameScene->CreateObject2D(new Label(L"Data\\Fonts\\algerian.spritefont", "0", XMFLOAT4(0, 0, 0, 1.0f)), "fps-counter");
	gameScene->GetObject2D<Label>("fps-counter")->SetColour(1.0f, 1.0f, 1.0f, 1.0f);

	gameScene->CreateObject2D(new Label(L"Data\\Fonts\\algerian.spritefont", "0 0 0", XMFLOAT4(0, 0, 0, 1.0f)), "worldpos");
	gameScene->GetObject2D("worldpos")->SetPosition(Vector2(0.f, 24.f));
	gameScene->GetObject2D<Label>("worldpos")->SetColour(1.0f, 1.0f, 1.0f, 1.0f);

	Audio::SetListener(&Graphics::Get()->camera.transform);

	engine->AddScene(titleScene, "title");
	engine->AddScene(gameScene, "game");

}

// Ideally don't want to use this, but it's here as an option
// Instead rely on the Object3D.Update(dTime) call for game logic updates
// 
// Create a new class inheriting from Object3D and override Update(float dTime)
// 
void GameUpdate(Engine* engine, float dTime) {
}