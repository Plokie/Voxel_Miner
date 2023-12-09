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
#include "HeldItem.h"

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
		InventoryItem& invItem = inventory->GetItemAt(slotNum, 0);
		HeldItem* heldItemModel = gameScene->GetObject3D<HeldItem>("HeldItem");
		if(invItem.ID == 0) {
			heldItemModel->SetItem((BlockID)0);
		}
		else {
			heldItemModel->SetItem(invItem.type, invItem.ID);
		}
	});

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
			Button* slotRect = (Button*)gameScene->CreateObject2D(new Button(XMFLOAT4( 0.3f, 0.3f, 0.3f, 1.0f )), "slot-" + to_string(row) + "-" + to_string(slot));
			slotRect->SetDimensions({ 55.f, 55.f });
			slotRect->SetDepth(12.f);

			slotRect->AddListener([this, slotRect] {
				//this->SlotPressed(slotRect);
			});

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

	void InventoryUI::ReleaseItem(ItemIcon* invItem) {
	for(int y = 0; y < INVSIZE_Y; y++) {
		for(int x = 0; x < INVSIZE_X; x++) {
			if(invSlots[x][y]->IsHovering()) {
				invItem->GetInvItem()->posX = x;
				invItem->GetInvItem()->posY = y;

				invItem->SetParent(invSlots[x][y]);
			}
		}
	}
	DrawHotbarIcons();
}

void InventoryUI::Open() {
	isOpen = true;
	invBg->SetEnabled(true);
	Input::SetMouseLocked(false);

	for(auto& invItem : this->inventory->GetInventoryItems()) {
		ItemIcon* icon = new ItemIcon(&invItem, this);
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

	for(auto& icon : _spawnedIcons) {
		icon->Update(dTime);
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
			ItemIcon* icon = new ItemIcon(&invItem, this);
			icon->Init(pDevice);
			icon->SetDimensions({ 50.f, 50.f });
			icon->SetAnchor({ .5f,.5f });
			icon->SetPivot({ .5f,.5f });

			icon->SetParent(hotbarSlots[invItem.posX]);

			_hotbarIcons.push_back(icon);
		}
	}
}
