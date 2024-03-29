#include "GameScene.h"

#include "../Engine/Engine.h"
#include "../Graphics/Graphics.h"
#include "Inventory.h"
#include "PlayerController.h"
#include "../Engine/UI/Label.h"
#include "../Engine/UI/UIRect.h"
#include "../Audio/Audio.h"
#include "HeldItem.h"
#include "ChunkManager.h"
#include "InventoryUI.h"
#include "PlayerData.h"

Scene* GameScene::Setup(Engine* engine, Scene* gameScene)
{
	gameScene->CreateObject3D(new ChunkManager(), "AChunkManager");
	//Inventory* inventory = (Inventory*)gameScene->CreateObject3D(new Inventory(), "Inventory");
	//inventory->LoadDefaultItems();

	PlayerData* playerData = (PlayerData*)gameScene->CreateObject3D(new PlayerData(), "PlayerData");
	playerData->GetInventory()->LoadDefaultItems();

	HeldItem* heldItem = (HeldItem*)gameScene->CreateObject3D(new HeldItem(), "HeldItem");
	PlayerController* pc = (PlayerController*)gameScene->CreateObject3D(new PlayerController(), "PlayerController");
	heldItem->transform.SetParent(&pc->transform);
	heldItem->transform.position = { .7f, -0.3f, 0.5f };

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
	playerData->AddOnScoreChangeEvent([scoreLabel](int score) {
		scoreLabel->SetText("Score: " + to_string(score));
	});


	UIRect* crosshair = (UIRect*)gameScene->CreateObject2D(new UIRect("crosshair", { 1.f,1.f,1.f,.7f }), "crosshair");
	crosshair->SetAnchor({ 0.5f, 0.5f });
	crosshair->SetPivot({ 0.5f, 0.5f });
	crosshair->SetDimensions({ 20.f, 20.f });

	gameScene->CreateObject2D(new InventoryUI(engine, gameScene), "invUI");

	Audio::SetListener(&Graphics::Get()->camera.transform);

	return gameScene;
}
