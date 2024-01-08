#include "FurnaceUI.h"

#include "BlockData.h"
#include "Inventory.h"
#include "ItemIcon.h"
#include "InventoryUI.h"
#include "../Graphics/Graphics.h"
#include "../Engine/UI/ProgressBar.h"

void FurnaceUI::Open(InterfaceContext ctx) {
	TableInterface::Open(ctx);
	
	Button* coalSlot = MakeSlot(0, 0);
	coalSlot->SetPosition(100.f, 130.f);
	TryMakeItemIcon(coalSlot, 0, 0);

	UIRect* coalIcon = new UIRect("coal-goes-here");
	coalIcon->Init(pDevice);
	coalIcon->InitSelf();
	coalIcon->SetDimensions({ 50.f, 50.f });
	coalIcon->SetPivot(0.5f, 0.5f);
	coalIcon->SetAnchor(0.5f, 0.5f);
	coalIcon->SetParent(coalSlot);
	_spawnedUIRects.push_back(coalIcon);

	/*UIRect* outputArrow = new UIRect("output-arrow");
	outputArrow->Init(pDevice);
	outputArrow->InitSelf();
	outputArrow->SetDimensions({ 100.f, 50.f });
	outputArrow->SetPosition({ 60.f, 0.f });
	outputArrow->SetPivot(0.5f, 0.5f);
	outputArrow->SetAnchor(0.5f, 0.5f);
	outputArrow->SetParent(this);
	_spawnedUIRects.push_back(outputArrow);*/


	Button* inputSlot = MakeSlot(1, 0);
	inputSlot->SetPosition(200.f, 130.f);
	//inputSlot->SetAnchor(0.5f, 0.5f);
	//inputSlot->SetPosition(0.f, 0.f);
	TryMakeItemIcon(inputSlot, 1, 0);

	Button* outputSlot = MakeSlot(2, 0);
	//outputSlot->SetPosition(360.f, 130.f);
	outputSlot->SetPosition(-100.f, 130.f);
	outputSlot->SetAnchor(1.0f, 0.f);
	TryMakeItemIcon(outputSlot, 2, 0);



	fuelProgress = new ProgressBar();
	fuelProgress->Init(pDevice);
	fuelProgress->SetParent(coalSlot);
	fuelProgress->SetPivot(0.5f, .5f);
	fuelProgress->SetAnchor(0.5f, 0.5f);
	fuelProgress->SetPosition(0.f, 50.f);
	fuelProgress->SetProgressCol({255.f/255.f, 81.f/255.f, 0.f, 1.f});
	fuelProgress->SetBgCol({ .3f, .3f, .3f, 1.f });
	fuelProgress->SetDimensions({ 110.f, 20.f });

	cookProgress = new ProgressBar();
	cookProgress->Init(pDevice);
	cookProgress->SetBgCol({ .3f, .3f, .3f, 1.f });
	cookProgress->SetDimensions({ 100.f, 20.f });
	cookProgress->SetPosition({ 50.f, 0.f });
	cookProgress->SetPivot(0.5f, 0.5f);
	cookProgress->SetAnchor(0.5f, 0.5f);
	cookProgress->SetParent(this);

	//cookProgress->SetParent(outputArrow);
	//cookProgress->SetPivot(0.5f, .5f);
	//cookProgress->SetAnchor(0.5f, 0.5f);
	//cookProgress->SetDimensions({ 100.f, 20.f });
	//cookProgress->SetPosition(0.f, 50.f);
}

void FurnaceUI::Close() {
	TableInterface::Close();

	delete fuelProgress;
	delete cookProgress;
}

void FurnaceUI::Start() {
}

void FurnaceUI::Update(const float dt){
	TableInterface::Update(dt);

	fuelProgress->SetValue(blockData->timer1 / 80.f);
	cookProgress->SetValue(blockData->timer2 / 10.f);
}

void FurnaceUI::Draw(SpriteBatch* spriteBatch) {
	TableInterface::Draw(spriteBatch);

	fuelProgress->Draw(spriteBatch);
	cookProgress->Draw(spriteBatch);
}
