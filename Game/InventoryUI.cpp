#include "InventoryUI.h"
#include "../Engine/Object2D.h"
#include "../Engine/UI/HorizontalLayoutRect.h"
#include "../Engine/UI/ProgressBar.h"
#include "../Engine/UI/Button.h"
#include "../Engine/Engine.h"
#include "ItemIcon.h"
#include "Inventory.h"
#include "InventoryItem.h"
#include "Items.h"
#include "Blocks.h"
#include "HeldItem.h"
//#include "CraftingUI.h"
#include "TableInterface.h"

InventoryUI::InventoryUI(Engine* engine, Scene* gameScene) {
	inventory = gameScene->GetObject3D<Inventory>("Inventory");
	this->SetDepth(15.f); // The item icons get drawn on the same draw call as the inventory, so make the depth ABOVE the other stuff

	inventory->AddOnChangeEvent([&] {DrawHotbarIcons(); });

	//

	hotbar = (HorizontalLayoutRect*)gameScene->CreateObject2D(new HorizontalLayoutRect({ 0.5f,0.5f,0.5f,0.7f }), "hotbar-parent");
	hotbar->SetPosition({ 0.0f, 0.0f });
	hotbar->SetDimensions({ (60.f * (INVSIZE_X + 1)) - 10.f, 70.f });
	hotbar->SetAnchor({ 0.0f, 0.0f });
	hotbar->SetPivot({ 0.0f, 0.0f });

	for(int i = 0; i < INVSIZE_X; i++) {
		string name = "hotbar-" + to_string(i);
		Button* slot = (Button*)gameScene->CreateObject2D(new Button(XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f)), name);
		slot->SetDimensions({ 60.f, 60.f });
		slot->SetDepth(1.f);

		hotbar->AddChild(slot);
		hotbarSlots[i] = slot;
	}

	hotbarSelect = (UIRect*)gameScene->CreateObject2D(new UIRect("white"), "hotbarSelect");
	hotbarSelect->SetParent(hotbar);
	hotbarSelect->SetDimensions({ 70.f,70.f });
	hotbarSelect->SetAnchor({ 0.f, 0.5f });
	hotbarSelect->SetPivot({ 0.f, 0.5f });
	hotbarSelect->SetDepth(0.5f);

	inventory->AddOnSelectEvent([this, gameScene](int slotNum) {

		hotbarSelect->SetPosition({ slotNum * 65.f, 0.f });
		InventoryItem* invItem = inventory->GetItemAt(slotNum, 0);
		HeldItem* heldItemModel = gameScene->GetObject3D<HeldItem>("HeldItem");
		if(invItem == nullptr || invItem->ID == 0) {
			heldItemModel->SetItem((BlockID)0);
		}
		else {
			heldItemModel->SetItem(invItem->type, invItem->ID);
		}
	});

	DrawHotbarIcons();


	ProgressBar* healthBar = (ProgressBar*)gameScene->CreateObject2D(new ProgressBar(), "healthBar");
	healthBar->SetAnchor({ 1.f, 0.f });
	healthBar->SetPivot({ 1.f, 0.f });
	healthBar->SetDimensions({ 340.f, 30.f });
	healthBar->SetPosition(-10.f, 10.f);
	healthBar->SetProgressCol({ 245.f/255.f, 66.f/255.f, 90.f/255.f, 1.f });
	healthBar->SetDirection(RIGHT_TO_LEFT);
	inventory->AddOnHealthChangeEvent([healthBar](int health) { //health out of 100
		healthBar->SetValue(health / (float)HEALTH_MAX);
	});

	UIRect* healthBarOverlay = (UIRect*)gameScene->CreateObject2D(new UIRect("bar-split"), "healthBarOverlay");
	healthBarOverlay->SetAnchor(0.f, 0.f);
	healthBarOverlay->SetPivot(0.f, 0.f);
	healthBarOverlay->SetDimensions({ 340.f, 30.f });
	healthBarOverlay->SetDepth(1.f);
	healthBarOverlay->SetParent(healthBar);

	ProgressBar* hungerBar = (ProgressBar*)gameScene->CreateObject2D(new ProgressBar(), "hungerBar");
	hungerBar->SetAnchor({ 1.f, 0.f });
	hungerBar->SetPivot({ 1.f, 0.f });
	hungerBar->SetDimensions({ 340.f, 30.f });
	hungerBar->SetPosition(-10.f, 50.f);
	hungerBar->SetProgressCol({ 143.f/255.f, 87.f/255.f, 24.f/255.f, 1.f });
	hungerBar->SetDirection(RIGHT_TO_LEFT);
	inventory->AddOnHungerChangeEvent([hungerBar](int hunger) { //hunger out of 100
		hungerBar->SetValue(hunger / (float)HUNGER_MAX);
	});

	UIRect* hungerBarOverlay = (UIRect*)gameScene->CreateObject2D(new UIRect("bar-split"), "hungerBarOverlay");
	hungerBarOverlay->SetAnchor(0.f, 0.f);
	hungerBarOverlay->SetPivot(0.f, 0.f);
	hungerBarOverlay->SetDimensions({ 340.f, 30.f });
	hungerBarOverlay->SetDepth(1.f);
	hungerBarOverlay->SetParent(hungerBar);




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
		rowRect->SetPosition({ 0.f, -(row * 60.f) - 35.f - ((row > 0) ? 10.f : 0.f) }); // the ternary operator makes it so theres a gap after the first row
		rowRect->SetAnchor({ 0.5f, 1.0f });
		rowRect->SetPivot({ 0.5f, 0.5f });
		rowRect->SetParent(invBg);
		rowRect->SetDepth(11.f);


		for(int slot = 0; slot < INVSIZE_X; slot++) {
			Button* slotRect = (Button*)gameScene->CreateObject2D(new Button(XMFLOAT4( 0.3f, 0.3f, 0.3f, 1.0f )), "slot-" + to_string(row) + "-" + to_string(slot));
			slotRect->SetDimensions({ 55.f, 55.f });
			slotRect->SetDepth(12.f);

			//slotRect->AddListener([this, slotRect] {
			//	//this->SlotPressed(slotRect);
			//});

			rowRect->AddChild(slotRect);

			invSlots[slot][row] = slotRect;
		}
	}

	/*CraftingUI* invCraftingUI = (CraftingUI*)gameScene->CreateObject2D(new CraftingUI(), "invCrafting");
	invCraftingUI->SetParent(invBg);
	invCraftingUI->SetPivot(0.5f, 0.0f);
	invCraftingUI->SetAnchor(0.5f, 0.f);
	invCraftingUI->SetDimensions({ 550.f, 265.f });
	invCraftingUI->SetPosition(0.f, 10.f);
	invCraftingUI->SetDepth(11.f);
	invCraftingUI->Create();*/
	//CraftingUI* invCraftingUI = (CraftingUI*)gameScene->CreateObject2D(new CraftingUI(), "invCrafting");
	//invCraftingUI->SetP
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

//void InventoryUI::SetHeldItem(ItemIcon* invItem) {
//	heldItem = invItem;
//}
//
//void InventoryUI::SlotPressed(Button* slot) {
//	if(heldItem != nullptr) {
//		InventoryItem* invItem = heldItem->GetInvItem();
//
//
//		heldItem->isHeld = false;
//		heldItem = nullptr;
//	}
//}

void InventoryUI::DeleteIcon(ItemIcon* itemIcon) {
	for(auto it = _spawnedIcons.begin(); it != _spawnedIcons.end(); it++) {
		if(*it == itemIcon) {
			delete *it;
			*it = nullptr;
			_spawnedIcons.erase(it);
			return;
		}
	}
}

//void InventoryUI::AttemptMoveFromStorage(ItemIcon* itemIcon) {
//
//	
//}

void InventoryUI::ReleaseItem(ItemIcon* invItem) {
	if(currentInterface) {
		currentInterface->ReleaseHeldItem();
	}

	for(int y = 0; y < INVSIZE_Y; y++) {
		for(int x = 0; x < INVSIZE_X; x++) {
			if(invSlots[x][y]->IsHovering()) {
				invItem->ReleaseFunction(x, y, invSlots[x][y], inventory, currentInterface);
			}
		}
	}
}



void InventoryUI::Open() {
	isOpen = true;
	invBg->SetEnabled(true);
	Input::SetMouseLocked(false);

	_spawnedIcons.reserve((INVSIZE_X * INVSIZE_Y) + INVSIZE_Y);

	for(auto& invItem : this->inventory->GetInventoryItems()) {
		ItemIcon* icon = new ItemIcon(invItem, this);
		icon->Init(pDevice);
		icon->SetDimensions({ 50.f, 50.f });
		icon->SetAnchor({ .5f,.5f });
		icon->SetPivot({ .5f,.5f });

		if(invItem->ID == 56797 || invItem->amount == -572662307 || invItem->posX == -572662307 || invItem->posY== -572662307 || invItem->type == 56797 || invItem->amount < -1) {
			//assert(false); // this will only be called on debug builds

			/*invItem->ID = 0;
			invItem->amount = 1;
			invItem->type = InventoryItem::ITEM;
			Vector2Int freeSpot = this->inventory->GetFreeSpot();
			invItem->posX = freeSpot.x;
			invItem->posY = freeSpot.y;*/
			//this isnt a fix, its just so the program doesnt crash so i can keep debugging and see what tf happened
			continue;
		}

		icon->SetParent(invSlots[invItem->posX][invItem->posY]);

		_spawnedIcons.push_back(icon);
	}
}

void InventoryUI::Open(TableInterface* tableInterface, InterfaceContext ctx) {
	Open();

	currentInterface = tableInterface;
	tableInterface->Open(ctx);
	tableInterface->SetParent(invBg);
}

void InventoryUI::Close() {
	isOpen = false;
	invBg->SetEnabled(false);
	Input::SetMouseLocked(true);

	if(currentInterface) {
		currentInterface->Close();
		delete currentInterface;
		currentInterface = nullptr;
	}

	//todo: throw held item to floor
	if(heldItem) {

	}
	heldItem = nullptr;

	for(auto& icon : _spawnedIcons) {
		delete icon;
	}
	_spawnedIcons.clear();
}

void InventoryUI::ReloadIcons()
{
	if(isOpen) {
		//todo: reload permanent icons
		// dont reload held item icon(s)
		
		for(auto& icon : _spawnedIcons) {
			icon->Reload();
		}

		if(heldItem) {
			heldItem->Reload();
		}

		//Close();
		//Open();
	}
}

void InventoryUI::Update(const float dTime) {
	if((Input::IsKeyPressed(VK_TAB) || Input::IsKeyPressed('I')) && (Input::IsMouseLocked() || isOpen)) {
		isOpen = !isOpen;
		if(isOpen) Open();
		else Close();
	}

	if(currentInterface) {
		currentInterface->Update(dTime);
	}

	for(auto& icon : _spawnedIcons) {
		if(icon == nullptr) 
			continue;
		icon->Update(dTime);
	}

}

void InventoryUI::Start() {
	Close();
}

void InventoryUI::Draw(SpriteBatch* spriteBatch) {
	if(currentInterface) {
		currentInterface->Draw(spriteBatch);
	}

	for(auto& itemIcon : _spawnedIcons) {
		itemIcon->Draw(spriteBatch);
	}

	for(auto& itemIcon : _hotbarIcons) {
		itemIcon->Draw(spriteBatch);
	}
}

bool InventoryUI::EraseIcon(ItemIcon* icon)
{
	for(auto it = _spawnedIcons.begin(); it != _spawnedIcons.end(); ++it) {
		if(*it == icon) {
			_spawnedIcons.erase(it);
			return true;
		}
	}
	return false;
}

void InventoryUI::DrawHotbarIcons() {
	for(ItemIcon*& icon : _hotbarIcons) {
		delete icon;
		icon = nullptr;
	}
	_hotbarIcons.clear();

	for(auto& invItem : inventory->GetInventoryItems()) {
		if(invItem->posY == 0) {
			ItemIcon* icon = new ItemIcon(invItem, this);
			icon->Init(pDevice);
			icon->SetDimensions({ 50.f, 50.f });
			icon->SetAnchor({ .5f,.5f });
			icon->SetPivot({ .5f,.5f });

			icon->SetParent(hotbarSlots[invItem->posX]);

			_hotbarIcons.push_back(icon);
		}
	}
}
