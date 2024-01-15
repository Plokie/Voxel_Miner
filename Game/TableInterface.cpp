#include "TableInterface.h"

#include "../Engine/UI/Button.h"
#include "ItemIcon.h"
#include "../Graphics/Graphics.h"
#include "InventoryUI.h"
#include "../Engine/Engine.h"
#include "BlockData.h"
#include "../Engine/UI/Label.h"
#include "../Engine/UI/TextInput.h"

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

Label* TableInterface::MakeLabel(const string& text)
{
	Label* newLabel = new Label("Baloo");
	newLabel->Init(Graphics::Get()->GetDevice());
	newLabel->InitSelf();
	newLabel->SetParent(this);
	newLabel->SetText(text);
	newLabel->SetAnchor(0.f, 0.f);
	newLabel->SetPivot(0.5f, 0.5f);

	_spawnedLabels.push_back(newLabel);

	return newLabel;
}

TextInput* TableInterface::MakeTextInput(const string& defaultText)
{
	TextInput* textInput = new TextInput("Baloo");
	textInput->Init(Graphics::Get()->GetDevice());
	textInput->InitSelf();
	textInput->SetDimensions({ 300.f, 35.f });
	textInput->SetParent(this);
	textInput->SetBgColour(0.2f,.2f,.2f,1.f);
	textInput->SetTextColour(1.f, 1.f, 1.f, 1.f);
	textInput->SetTemplateText(defaultText);
	textInput->SetAnchor(0.f, 0.f);
	textInput->SetPivot(0.0f, 0.0f);

	invUI = Engine::Get()->GetCurrentScene()->GetObject2D<InventoryUI>("invUI");

	_spawnedTextInputs.push_back(textInput);

	return textInput;
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

Button* TableInterface::MakeButton(const string& text, const XMFLOAT4& textCol, const XMFLOAT4& bgCol) {
	Button* button = new Button("Baloo");
	button->Init(pDevice);
	button->InitSelf();
	button->SetDimensions({ 150.f, 35.f });
	button->SetParent(this);
	button->SetAnchor(0.f, 0.f);
	button->SetPivot(0.f, 0.f);
	button->SetText(text);
	button->SetTextColour(textCol.x, textCol.y, textCol.z, textCol.w);
	button->SetBgColour(bgCol.x, bgCol.y, bgCol.z, bgCol.w);

	_spawnedButtons.push_back(button);
	return button;
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
	if(invUI == nullptr) return;
	if(invUI->heldItem == nullptr) return;

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
	invUI->canClose = true;

	for(auto& icon : _spawnedItemIcons) { // attempts to pick up
		icon->Update(dt);
	}
	for(auto& rect : _spawnedUIRects) {
		rect->Update(dt);
	}
	for(auto& slot : _spawnedSlots) { // drops it
		slot->Update(dt);
	}
	for(auto& label : _spawnedLabels) {
		label->Update(dt);
	}
	for(auto& textInput : _spawnedTextInputs) {
		textInput->Update(dt);

		if(textInput->IsEngaged()) invUI->canClose = false;
	}
	for(auto& button : _spawnedButtons) {
		button->Update(dt);
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
	itemExistsHash.clear();

	for(auto& label : _spawnedLabels) {
		delete label;
		label = nullptr;
	}
	_spawnedLabels.clear();

	for(auto& textInput : _spawnedTextInputs) {
		delete textInput;
		textInput = nullptr;
	}
	_spawnedTextInputs.clear();

	for(auto& button : _spawnedButtons) {
		delete button;
		button = nullptr;
	}
	_spawnedButtons.clear();
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

	for(auto& button : _spawnedButtons) {
		button->Draw(sb);
	}

	for(auto& icon : _spawnedUIRects) {
		icon->Draw(sb);
	}

	for(auto& icon : _spawnedItemIcons) {
		icon->Draw(sb);
	}

	for(auto& label : _spawnedLabels) {
		label->Draw(sb);
	}

	for(auto& textInput : _spawnedTextInputs) {
		textInput->Draw(sb);
	}

}
