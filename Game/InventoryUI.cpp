#include "InventoryUI.h"
#include "../Engine/Object2D.h"
#include "../Engine/UI/HorizontalLayoutRect.h"
#include "../Engine/UI/Button.h"
#include "../Engine/Engine.h"
#include "ItemIcon.h"
#include "Inventory.h"
#include "InventoryItem.h"
#include "Items.h"
#include "Blocks.h"

InventoryUI::InventoryUI(Engine* engine, Scene* gameScene) {
	inventory = gameScene->GetObject3D<Inventory>("Inventory");
	this->SetDepth(15.f); // The item icons get drawn on the same draw call as the inventory, so make the depth ABOVE the other stuff


	// TODO: TEMP TEST INVENTORY ITEMS
	inventory->AddItem(ItemID::AMETHYST_PICKAXE, 2);
	inventory->AddItem(ItemID::RAW_GOLD, 32);
	inventory->AddItem(ItemID::RAW_COPPER, 12);
	inventory->AddItem(ItemID::TITANIUM_BAR, 5);

	inventory->AddItem(BlockID::GOLD_ORE);
	inventory->AddItem(BlockID::GRASS, 14);
	inventory->AddItem(BlockID::OAK_LOG, 10);
	inventory->AddItem(BlockID::OAK_PLANKS, 93);

	inventory->AddOnChangeEvent([&] {DrawHotbarIcons(); });

	//

	hotbar = (HorizontalLayoutRect*)gameScene->CreateObject2D(new HorizontalLayoutRect({ 0.5f,0.5f,0.5f,0.7f }), "hotbar-parent");
	//hotbar->SetPosition({ 0.0f, -10.0f });
	hotbar->SetPosition({ 0.0f, 0.0f });
	hotbar->SetDimensions({ (60.f * (INVSIZE_X + 1)) - 10.f, 70.f });

	//hotbar->SetAnchor({ 0.5f, 1.0f });
	//hotbar->SetPivot({0.5f, 1.0f});

	hotbar->SetAnchor({ 0.0f, 0.0f });
	hotbar->SetPivot({ 0.0f, 0.0f });

	for(int i = 0; i < INVSIZE_X; i++) {
		string name = "hotbar-" + to_string(i);
		UIRect* slot = (UIRect*)gameScene->CreateObject2D(new UIRect("white", { 0.3f, 0.3f, 0.3f, 0.7f }), name);
		slot->SetDimensions({ 60.f, 60.f });
		slot->SetDepth(1.0f);

		hotbar->AddChild(slot);
		hotbarSlots[i] = slot;
	}

	DrawHotbarIcons();


	invBg = (UIRect*)gameScene->CreateObject2D(new UIRect("white", { 0.5f,0.5f,0.5f,1.0f }), "invBg");
	invBg->SetDimensions({ 570.f, 600.f });
	invBg->SetPivot({ 0.5f, 0.5f });
	invBg->SetAnchor({ 0.5f, 0.5f });
	invBg->SetDepth(10.f);
	invBg->SetEnabled(false);

	for(int row = 0; row < INVSIZE_Y; row++) {
		HorizontalLayoutRect* rowRect = (HorizontalLayoutRect*)gameScene->CreateObject2D(new HorizontalLayoutRect({ 0.4f,0.4f,0.4f,1.0f }), "row" + to_string(row));
		rowRect->SetDimensions({ 55.f * 10, 60.f   });
		//rowRect->SetPosition({ 10.f, (row * 50.f) + 10.f });
		rowRect->SetPosition({ 0.f, -(row * 60.f) - 35.f - ((row > 0) ? 10.f : 0.f) });
		rowRect->SetAnchor({ 0.5f, 1.0f });
		rowRect->SetPivot({ 0.5f, 0.5f });
		rowRect->SetParent(invBg);
		rowRect->SetDepth(11.f);


		for(int slot = 0; slot < INVSIZE_X; slot++) {
			UIRect* slotRect = (UIRect*)gameScene->CreateObject2D(new UIRect("white", {0.3f, 0.3f, 0.3f, 1.0f}), "slot-" + to_string(row) + "-" + to_string(slot));
			slotRect->SetDimensions({55.f, 55.f});
			slotRect->SetDepth(12.f);

			rowRect->AddChild(slotRect);

			invSlots[slot][row] = slotRect;
		}
	}
}

InventoryUI::~InventoryUI() {
	for(ItemIcon*& rect : _spawnedIcons) {
		delete rect;
		rect = nullptr;
	}

	for(ItemIcon*& rect : _hotbarIcons) {
		delete rect;
		rect = nullptr;
	}
}



void InventoryUI::Open() {
	isOpen = true;
	invBg->SetEnabled(true);
	Input::SetMouseLocked(false);

	for(const InventoryItem& invItem : this->inventory->GetInventoryItems()) {
		//const string& atlasName = (invItem.type == InventoryItem::Type::BLOCK) ? "atlas" : "item-atlas";

		//UIRect* icon = new UIRect(atlasName, { 1.f,1.f,1.f,1.f });
		//icon->Init(pDevice);
		//icon->SetDimensions({ 50.f,50.f });
		//icon->SetAnchor({ .5f,.5f });
		//icon->SetPivot({ .5f,.5f });
		//icon->SetDepth(13.f);

		//Vector2Int uvPos = invItem.GetUVPos();
		//LONG texPosX = static_cast<LONG>(ITEM_ATLAS_TILE_SIZE * uvPos.x);
		//LONG texPosY = static_cast<LONG>(ITEM_ATLAS_TILE_SIZE * uvPos.y);

		//icon->SetTexRect({
		//	texPosX, texPosY,
		//	texPosX + ITEM_ATLAS_TILE_SIZE,
		//	texPosY + ITEM_ATLAS_TILE_SIZE
		//});

		ItemIcon* icon = new ItemIcon(&invItem);
		icon->Init(pDevice);
		icon->SetDimensions({ 50.f, 50.f });
		icon->SetAnchor({ .5f,.5f });
		icon->SetPivot({ .5f,.5f });

		icon->SetParent(invSlots[invItem.posX][invItem.posY]);

		_spawnedIcons.push_back(icon);
	}
	
}

void InventoryUI::Close() {
	isOpen = false;
	invBg->SetEnabled(false);
	Input::SetMouseLocked(true);

	for(auto& icon : _spawnedIcons) {
		delete icon;
	}
	_spawnedIcons.clear();
}

void InventoryUI::Update(const float dTime) {
	if(Input::IsKeyPressed(VK_TAB) || Input::IsKeyPressed('I')) {
		isOpen = !isOpen;
		if(isOpen) Open();
		else Close();
	}
}

void InventoryUI::Start() {
	Close();
}

void InventoryUI::Draw(SpriteBatch* spriteBatch) {
	for(auto& itemIcon : _spawnedIcons) {
		itemIcon->Draw(spriteBatch);
	}

	for(auto& itemIcon : _hotbarIcons) {
		itemIcon->Draw(spriteBatch);
	}
}

void InventoryUI::DrawHotbarIcons() {
	for(ItemIcon*& icon : _hotbarIcons) {
		delete icon;
		icon = nullptr;
	}
	_hotbarIcons.clear();

	for(auto& invItem : inventory->GetInventoryItems()) {
		if(invItem.posY == 0) {
			//const string& atlasName = (invItem.type == InventoryItem::Type::BLOCK) ? "atlas" : "item-atlas";
			//UIRect* slotIcon = new UIRect(atlasName, {1.0f, 1.0f, 1.0f, 1.0f});
			//slotIcon->Init(pDevice);
			//slotIcon->SetDimensions({ 50.f, 50.f });
			//slotIcon->SetPivot({ 0.5f, 0.5f });
			//slotIcon->SetAnchor({ 0.5f, 0.5f });
			//slotIcon->SetDepth(2.0f);


			//Vector2Int uvPos = invItem.GetUVPos();
			//LONG texPosX = static_cast<LONG>(ITEM_ATLAS_TILE_SIZE * uvPos.x);
			//LONG texPosY = static_cast<LONG>(ITEM_ATLAS_TILE_SIZE * uvPos.y);

			//slotIcon->SetTexRect({
			//	texPosX, texPosY,
			//	texPosX + ITEM_ATLAS_TILE_SIZE,
			//	texPosY + ITEM_ATLAS_TILE_SIZE
			//});

			ItemIcon* icon = new ItemIcon(&invItem);
			icon->Init(pDevice);
			icon->SetDimensions({ 50.f, 50.f });
			icon->SetAnchor({ .5f,.5f });
			icon->SetPivot({ .5f,.5f });

			icon->SetParent(hotbarSlots[invItem.posX]);

			_hotbarIcons.push_back(icon);
		}
	}
}
