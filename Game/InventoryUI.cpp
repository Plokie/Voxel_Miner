#include "InventoryUI.h"
#include "../Engine/Object2D.h"
#include "../Engine/UI/HorizontalLayoutRect.h"
#include "../Engine/UI/Button.h"
#include "../Engine/Engine.h"

InventoryUI::InventoryUI(Engine* engine, Scene* gameScene) {
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

		UIRect* slotIcon = (UIRect*)gameScene->CreateObject2D(new UIRect("head", { 1.0f, 1.0f, 1.0f, 1.0f }), name + "-icon");
		slotIcon->SetDimensions({ 50.f, 50.f });
		slotIcon->SetPivot({ 0.5f, 0.5f });
		slotIcon->SetAnchor({ 0.5f, 0.5f });
		slotIcon->SetDepth(2.0f);
		slotIcon->SetParent(slot);
	}


	invBg = (UIRect*)gameScene->CreateObject2D(new UIRect("white", { 0.5f,0.5f,0.5f,1.0f }), "invBg");
	invBg->SetDimensions({ 600.f, 450.f });
	invBg->SetPivot({ 0.5f, 0.5f });
	invBg->SetAnchor({ 0.5f, 0.5f });
	invBg->SetDepth(10.f);

	for(int row = 0; row < INVSIZE_Y; row++) {
		HorizontalLayoutRect* rowRect = (HorizontalLayoutRect*)gameScene->CreateObject2D(new HorizontalLayoutRect({ 0.4f,0.4f,0.4f,1.0f }), "row" + to_string(row));
		rowRect->SetDimensions({ 55.f * 10, 60.f   });
		//rowRect->SetPosition({ 10.f, (row * 50.f) + 10.f });
		rowRect->SetPosition({ 0.f, (row * 60.f)});
		rowRect->SetAnchor({ 0.5f, 0.5f });
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

void InventoryUI::Open() {
	isOpen = true;
	invBg->SetEnabled(true);
	Input::SetMouseLocked(false);


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
		if(isOpen) {
			Open();
		}
		else {
			Close();
		}
	}
}

void InventoryUI::Start() {
	Close();
}
