#include "GameStart.h"

#include "../Audio/Audio.h"

// ---------------------------------------
#include "TitleScreen.h"
#include "PauseScreen.h"
#include "HeldItem.h"
#include "Meshes/HeldItemMesh.h"
#include "Meshes/DroppedItemMesh.h"
#include "GameScene.h"
// ---------------------------------------


void LoadResources() {
	// todo: move resource loading to a separate thread, so the main thread can display a loading screen
	//Resources::LoadTexture(L"Data\\Textures\\img.dds", "head");
	Resources::LoadTexture(L"Data\\Textures\\crosshair.dds", "crosshair");
	//Resources::LoadTexture(L"Data\\Textures\\pfp.dds", "pfp");
	Resources::LoadTexture(L"Data\\Textures\\block-select.dds", "block-select");
	Resources::LoadTexture(L"Data\\Textures\\chunkborder.dds", "chunkborder");
	Resources::LoadTexture(L"Data\\Textures\\bar-split.dds", "bar-split");
	//Resources::LoadTexture(L"Data\\Textures\\grass2.dds", "grass");
	Resources::LoadTexture(L"Data\\Textures\\atlas.dds", "atlas");
	Resources::LoadTexture(L"Data\\Textures\\item-atlas.dds", "item-atlas");

	Resources::LoadTexture(L"Data\\Textures\\coal-goes-here.dds", "coal-goes-here");
	Resources::LoadTexture(L"Data\\Textures\\output-arrow.dds", "output-arrow");

	Resources::LoadFont(L"Data\\Fonts\\Baloo2.spritefont", "Baloo");

	Resources::LoadPixelShader(L"Data\\Shaders\\pixelshader.cso", "pixelshader");
	Resources::LoadPixelShader(L"Data\\Shaders\\pixelshellgrass.cso", "pixelshellgrass");
	Resources::LoadPixelShader(L"Data\\Shaders\\chunkpixelshader.cso", "chunkpixelshader");
	//Resources::LoadPixelShader(L"demopixelshader.cso", "demopshader");

	Resources::LoadVertexShader(L"Data\\Shaders\\vertexshader.cso", "vertexshader");
	Resources::LoadVertexShader(L"Data\\Shaders\\watervertexshader.cso", "watervertexshader");
	Resources::LoadVertexShader(L"Data\\Shaders\\vertexshellgrass.cso", "vertexshellgrass");
	Resources::LoadVertexShader(L"Data\\Shaders\\vertexshaderleaves.cso", "vertexshaderleaves");

	Audio::LoadClipStream("Data\\Music\\MusicBg.mp3", "MusicBg");
	Audio::LoadClip("Data\\Sounds\\hit.wav", "hit");
	Audio::LoadClip("Data\\Sounds\\hurt.wav", "hurt");
	Audio::LoadClip("Data\\Sounds\\eat.wav", "eat");

	Audio::Play("MusicBg", 1.f);

	Resources::LoadMesh("cube");
	Resources::LoadMesh(exampleFloorVertices, ARRAYSIZE(exampleFloorVertices), exampleCubeIndices, ARRAYSIZE(exampleCubeIndices), "floorMesh");
	Resources::LoadMesh(chunkBorderVertices, ARRAYSIZE(chunkBorderVertices), exampleCubeIndices, ARRAYSIZE(exampleCubeIndices), "chunkborder");
	//Resources::LoadMesh(exampleInverseCubeVertices, ARRAYSIZE(exampleInverseCubeVertices), exampleInverseCubeIndices, ARRAYSIZE(exampleInverseCubeIndices), "inverse-cube");
	Resources::LoadMesh(exampleInverseCubeVertices, ARRAYSIZE(exampleInverseCubeVertices), exampleInverseCubeIndices, ARRAYSIZE(exampleInverseCubeIndices), "inverse-cube");

	Resources::LoadMesh(heldItemVertices, ARRAYSIZE(heldItemVertices), heldItemIndices, ARRAYSIZE(heldItemIndices), "heldItemMesh");
	Resources::LoadMesh(droppedItemVertices, ARRAYSIZE(droppedItemVertices), droppedItemIndices, ARRAYSIZE(droppedItemIndices), "droppedItemMesh");
}


// Call game-related initialisation stuff from here
void GameStart(Engine* engine) {

	LoadResources();

	//todo: better randomness. Maybe random singleton for World / Block / Entity randomness
	srand((unsigned)time(NULL));

	Input::SetVirtualMouse(true);
	TitleScreen::Setup(engine);
	PauseScreen::Setup(engine);
	
	Scene* gameScene = GameScene::Setup(engine, new Scene(Graphics::Get()));
	
	engine->AddScene(gameScene, "game");
}

// Ideally don't want to use this, but it's here as an option
// Instead rely on the Object3D.Update(dTime) call for game logic updates
// 
// Create a new class inheriting from Object3D and override Update(float dTime)
// 
void GameUpdate(Engine* engine, float dTime) {
	Transform* sunTransform = &Graphics::Get()->sun;
	sunTransform->rotation.x += dTime * (3.14159265359f / 600.f);
	if(sunTransform->rotation.x > 3.14159265359f)
		sunTransform->rotation.x = 0.f;
}