#include "TableInterface.h"

#include "../Engine/UI/Button.h"
#include "ItemIcon.h"
#include "../Graphics/Graphics.h"
#include "InventoryUI.h"
#include "../Engine/Engine.h"
#include "BlockData.h"

Button* TableInterface::MakeSlot(int idx, int idy)
{
	Button* slot = new Button(XMFLOAT4(.3f, .3f, .3f, 1.f));
	slot->Init(Graphics::Get()->GetDevice());
	slot->SetDimensions({ 55.f, 55.f });
	slot->SetDepth(12.f);
	slot->SetParent(this);
	slot->SetAnchor(0.f, 0.f);
	slot->SetPivot(0.5f, .5f);

	invUI = Engine::Get()->GetCurrentScene()->GetObject2D<InventoryUI>("invUI");
	//slot->AddListener([this] {

	//	//	InventoryUI* invUI = Engine::Get()->GetCurrentScene()->GetObject2D<InventoryUI>("invUI");
	//	//if(invUI->heldItem) {
	//	//	// turn held item into a local item icon
	//	//	invUI->heldItem->isHeld = false;
	//	//	invUI->heldItem = nullptr;
	//	//}
	//	ReleaseHeldItem();


	//});

	_indexedSlots[{idx, idy}] = slot;
	_spawnedSlots.push_back(slot);


	return slot;
}

ItemIcon* TableInterface::MakeItemIcon(InventoryItem* invItem)
{
	ItemIcon* icon = new ItemIcon(invItem, invUI); //todo: easier MakeIcon func
	icon->Init(pDevice);
	icon->SetDimensions({ 50.f, 50.f });
	icon->SetAnchor({ .5f,.5f });
	icon->SetPivot({ .5f,.5f });
	icon->SetInventoryParent(blockData->blockInventory);

	return icon;
}

void TableInterface::ComputeItemExistsHash() {
	if(blockData && blockData->blockInventory)
	for(auto& invItem : blockData->blockInventory->GetInventoryItems()) {
		itemExistsHash[{invItem->posX, invItem->posY}] = invItem;
	}
}

bool TableInterface::TryMakeItemIcon(Button* parent, int idx, int idy)
{
	auto itFind = itemExistsHash.find({ idx, idy });
	if(itFind != itemExistsHash.end()) {
		ItemIcon* icon = MakeItemIcon(itFind->second);
		icon->SetParent(parent);

		_spawnedItemIcons.push_back(icon);
		return true;
	}
	return false;
}

void TableInterface::ReleaseHeldItem() {
	if(invUI->heldItem == nullptr) return;

	//todo: essentially re-write InventoryUI::ReleaseItem's functionality

	for(const auto& kvp : _indexedSlots) {
		if(kvp.second == nullptr) continue;

		if(kvp.second->IsHovering()) {
			invUI->heldItem->ReleaseFunction(get<0>(kvp.first), get<1>(kvp.first), kvp.second, blockData->blockInventory, this);
		}
	}
}

bool TableInterface::EraseIcon(ItemIcon* icon) {
	for(auto it = _spawnedItemIcons.begin(); it != _spawnedItemIcons.end(); ++it) {
		if(*it == icon) {
			_spawnedItemIcons.erase(it);
			return true;
		}
	}
	return false;
}

void TableInterface::ReloadIcons() {
	for(auto& icon : _spawnedItemIcons) {
		icon->Reload();
	}
}

void TableInterface::HardReload()
{
	Close();

	if(blockData->blockInventory) {
		blockData->blockInventory->ClearEmptyItems();
	}

	Open(recentContext);
}

void TableInterface::PushIcon(ItemIcon* itemIcon)
{
	_spawnedItemIcons.push_back(itemIcon);
}

void TableInterface::Update(const float dt) {
	for(auto& icon : _spawnedItemIcons) { // attempts to pick up
		icon->Update(dt);
	}
	for(auto& rect : _spawnedUIRects) {
		rect->Update(dt);
	}
	for(auto& slot : _spawnedSlots) { // drops it
		slot->Update(dt);
	}


	//if(invUI->heldItem) {
	//	if(!invUI->heldItem->isHeld) {
	//		invUI->heldItem = nullptr;
	//	}
	//}
}
void TableInterface::Close()
{
	for(auto& slot : _spawnedSlots) {
		delete slot;
		slot = nullptr;
	}
	_spawnedSlots.clear();
	for(auto& rect : _spawnedUIRects) {
		delete rect;
		rect = nullptr;
	}
	_spawnedUIRects.clear();
	for(auto& itemIcon : _spawnedItemIcons) {
		delete itemIcon;
		itemIcon = nullptr;
	}
	_spawnedItemIcons.clear();
}

void TableInterface::Open(InterfaceContext ctx) {
	this->Init(Graphics::Get()->GetDevice());
	this->InitSelf(); //overriden func to init child-specific features
	this->blockData = ctx.blockData;

	recentContext = ctx;

	this->SetPivot(0.5f, 0.0f);
	this->SetAnchor(0.5f, 0.f);
	this->SetDimensions({ 550.f, 265.f });
	this->SetPosition(0.f, 10.f);
	this->SetDepth(11.f);

	ComputeItemExistsHash();
}


void TableInterface::Draw(SpriteBatch* sb) {
	for(auto& slot : _spawnedSlots) {
		slot->Draw(sb);
	}

	for(auto& icon : _spawnedUIRects) {
		icon->Draw(sb);
	}

	for(auto& icon : _spawnedItemIcons) {
		icon->Draw(sb);
	}

}
