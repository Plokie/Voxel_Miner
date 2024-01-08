#include "FurnaceUI.h"

#include "BlockData.h"
#include "Inventory.h"
#include "ItemIcon.h"
#include "InventoryUI.h"
#include "../Graphics/Graphics.h"

void FurnaceUI::Open(InterfaceContext ctx) {
	TableInterface::Open(ctx);
	
	
	Button* coalSlot = MakeSlot(0, 0);
	coalSlot->SetPosition(100.f, 130.f);
	TryMakeItemIcon(coalSlot, 0, 0);

	UIRect* coalIcon = new UIRect("coal-goes-here");
	coalIcon->Init(pDevice);
	coalIcon->InitSelf();
	coalIcon->SetDimensions({ 40.f, 40.f });
	coalIcon->SetPivot(0.5f, 0.5f);
	coalIcon->SetAnchor(0.5f, 0.5f);
	coalIcon->SetParent(coalSlot);
	_spawnedUIRects.push_back(coalIcon);


	Button* inputSlot = MakeSlot(1, 0);
	inputSlot->SetPosition(160.f, 130.f);
	//inputSlot->SetAnchor(0.5f, 0.5f);
	//inputSlot->SetPosition(0.f, 0.f);
	TryMakeItemIcon(inputSlot, 1, 0);

	Button* outputSlot = MakeSlot(2, 0);
	//outputSlot->SetPosition(360.f, 130.f);
	outputSlot->SetPosition(-100.f, 130.f);
	outputSlot->SetAnchor(1.0f, 0.f);
	TryMakeItemIcon(outputSlot, 2, 0);
}

void FurnaceUI::Close() {
	TableInterface::Close();
}

void FurnaceUI::Start() {
}

void FurnaceUI::Update(const float dt){
	TableInterface::Update(dt);
}

void FurnaceUI::Draw(SpriteBatch* spriteBatch) {
	TableInterface::Draw(spriteBatch);
}
