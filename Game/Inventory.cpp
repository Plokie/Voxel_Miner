#include "Inventory.h"

#include "Crafting.h"
#include "InventoryUI.h"
#include "../Engine/Engine.h"
#include "../Audio/Audio.h"
#include "DroppedItem.h"

//map<HUNGER_DECREMENT_STATE, bool> Inventory::_hungerDecrementStates 
//map<HUNGER_DECREMENT_STATE, bool> Inventory::_hungerDecrementStates = {
//	{HDS_WALK, false},
//	{HDS_SPRINT, false},
//	{HDS_JUMP, false},
//};

vector<InventoryItem*> Inventory::GetItemsWithTags(UINT32 tags) {
	vector<InventoryItem*> retVec = {};

	for(InventoryItem* invItem : items) {
		if(invItem->type == InventoryItem::ITEM) {
			Item itemDef = ItemDef::Get(static_cast<ItemID>(invItem->ID));
			if(itemDef.GetTags() & tags) {
				retVec.push_back(invItem);
			}
		}
	}


	return retVec;
}

const Vector2Int Inventory::GetFreeSpot() const {
	Vector2Int tryPos = { 0, 0 };

	while(DoesItemExistAtPos(tryPos.x, tryPos.y)) {
		tryPos += Vector2Int(1, 0);

		if(tryPos.x >= INVSIZE_X) { // mod didnt work and i dont have the patience to figure out that i had to like add 1 or something dumb
			tryPos.x = 0;
			tryPos.y += 1;
		}
	} 

	if(tryPos.y >= INVSIZE_Y) return { -1, -1 };

	return tryPos;
}

bool Inventory::GetHeldItem(InventoryItem** out)
{
	for(InventoryItem*& invItem : items) {
		if(invItem->posX == selectedSlotNum && invItem->posY == 0) {
			*out = invItem;
			return true;
		}
	}
	return false;
}

bool Inventory::GetItemAt(int x, int y, InventoryItem** out)
{
	for(InventoryItem* invItem : items) {
		if(invItem->posX == x && invItem->posY == y) {
			*out = invItem;
			return true;
		}
	}
	*out = nullptr;
	return false;
}

const bool Inventory::DoesItemExistAtPos(int posX, int posY) const
{
	for(const auto& invItem : items) {
		if(invItem->posX == posX && invItem->posY == posY) return true;
	}
	return false;
}
bool Inventory::AddItem(const InventoryItem* item, InventoryItem** outInvItem) {
	if(item->type == InventoryItem::Type::BLOCK) {
		return AddItem((BlockID)item->ID, item->amount, outInvItem);
	}
	else {
		return AddItem((ItemID)item->ID, item->amount, outInvItem);
	}
}

bool Inventory::AddItem(const BlockID blockID, const int amount, InventoryItem** outInvItem) {
	return AddItem(blockID, InventoryItem::Type::BLOCK, amount, outInvItem);
}


bool Inventory::AddItem(const ItemID itemID, const int amount, InventoryItem** outInvItem) {
	return AddItem(itemID, InventoryItem::Type::ITEM, amount, outInvItem);
}

bool Inventory::AddItem(const unsigned int ID, const InventoryItem::Type type, const int amount, InventoryItem** outInvItem) {
	//if(outRemainder != nullptr) *outRemainder = amount; //hmm

	for(auto& invItem : items) {
		// check for merges
		int maxStack = (invItem->type == InventoryItem::Type::ITEM) ? ItemDef::Get((ItemID)invItem->ID).GetMaxStack() : MAX_STACK;
		if(invItem->ID == ID && invItem->type == type && invItem->amount < maxStack) { // able to merge
			invItem->amount += amount;

			if(invItem->amount > maxStack) {
				AddItem(ID, type, invItem->amount - amount); // add remainder
				invItem->amount = maxStack;
			}
			else 
			{
				InvokeOnChange();
				InvokeOnSelect();
			}

			if(outInvItem) *outInvItem = invItem;

			return true;
		}
	}



	// need to split stack here
	int maxStack = (type == InventoryItem::Type::ITEM) ? ItemDef::Get((ItemID)ID).GetMaxStack() : MAX_STACK;

	if(amount <= maxStack) {
		Vector2Int newPos = GetFreeSpot();
		if(newPos.x == -1) return false;
		//items.emplace_back(type, ID, newPos.x, newPos.y, amount);
		InventoryItem* newInvItem = new InventoryItem(type, ID, newPos.x, newPos.y, amount);
		items.push_back(newInvItem);
		if(outInvItem) *outInvItem = newInvItem;
	}
	else {
		int fullstacks = amount / maxStack;
		int remainder = amount % maxStack;

		for(int i = 0; i < fullstacks; i++) {
			Vector2Int newPos = GetFreeSpot();
			if(newPos.x == -1) return false;
			//items.emplace_back(type, ID, newPos.x, newPos.y, maxStack);
			InventoryItem* newInvItem = new InventoryItem(type, ID, newPos.x, newPos.y, maxStack);
			items.push_back(newInvItem);
			if(outInvItem) *outInvItem = newInvItem;
		}

		if(remainder > 0) {
			Vector2Int newPos = GetFreeSpot();
			if(newPos.x == -1) return false;
			//items.emplace_back(type, ID, newPos.x, newPos.y, remainder);
			InventoryItem* newInvItem = new InventoryItem(type, ID, newPos.x, newPos.y, remainder);
			items.push_back(newInvItem);
			if(outInvItem) *outInvItem = newInvItem;
		}
	}

	
	
	
	InvokeOnChange();
	InvokeOnSelect();
	return true;
}

void Inventory::PushItem(InventoryItem* item) {
	items.push_back(item);
}

void Inventory::SubItem(const BlockID blockID, const int amount) {
	SubItem(blockID, InventoryItem::Type::BLOCK, amount);
}

void Inventory::SubItem(const ItemID itemID, const int amount) {
	SubItem(itemID, InventoryItem::Type::ITEM, amount);
}

void Inventory::SubItem(const unsigned int ID, const InventoryItem::Type type, const int amount) {
	for(auto it = items.begin(); it != items.end(); it++) {
		InventoryItem*& invItem = *it;
		if(invItem->ID == ID && invItem->type == type) {

			//todo: if sub more than a stack, sub remainder too
			invItem->amount -= amount;
			if(invItem->amount <= 0) {
				delete *it;
				items.erase(it);
			}
			InvokeOnChange();
			InvokeOnSelect();
			return;
		}
	}
}

void Inventory::SubHeldItem(int amount, InventoryItem* heldInvItem)
{
	if(heldInvItem == nullptr) {
		if(!GetHeldItem(&heldInvItem)) return;
	}

	heldInvItem->amount -= amount;
	if(heldInvItem->amount <= 0) {
		//items.erase(it);
		ClearEmptyItems();
	}
	InvokeOnChange();
	InvokeOnSelect();

}

int Inventory::GetItemCount(const BlockID blockID)
{
	return GetItemCount(blockID, InventoryItem::Type::BLOCK);
}

int Inventory::GetItemCount(const ItemID itemID)
{
	return GetItemCount(itemID, InventoryItem::Type::ITEM);
}

int Inventory::GetItemCount(const unsigned int ID, const InventoryItem::Type type)
{
	int count = 0;
	for(const InventoryItem* item : items) {
		if(item->ID == ID && item->type == type) count += item->amount;
	}

	return count;
}

void Inventory::ClearEmptyItems() {

	//return; //todo: remove me after debugging
	for(vector<InventoryItem*>::iterator it = items.begin(); it != items.end();) {
		if((*it)->amount <= 0) {
			delete *it;
			it = items.erase(it);
		}
		else {
			++it;
		}
	}

	//InventoryUI* inv = Engine::Get()->GetCurrentScene()->GetObject2D<InventoryUI>("invUI");
	//inv->ReloadIcons();
}

bool Inventory::CanCraft(const Recipe& recipe) {
	for(const RecipeComponent& component : recipe.input) {
		if(GetItemCount(component.ID, component.type) < component.amount) return false;
	}

	return true;
}

bool Inventory::TryCraft(const Recipe& recipe) {
	if(CanCraft(recipe)) {
		for(const RecipeComponent& component : recipe.input) {
			SubItem(component.ID, component.type, component.amount);
		}

		AddItem(recipe.result.ID, recipe.result.type, recipe.result.amount);
		InvokeOnChange();
		InvokeOnSelect();
		
		return true;
	}

	return false;
}

nlohmann::json Inventory::Serialize()
{
	nlohmann::json json = {};
	
	vector<nlohmann::json> itemJsons = {};
	for(auto& invItem : items) {
		itemJsons.push_back(invItem->Serialize());
	}
	json["items"] = itemJsons;

	//todo: move to PlayerData class
	
	//json[""]

	return json;
}

void Inventory::Deserialize(nlohmann::json jsonInv) {
	items.clear();
	items.reserve((INVSIZE_X * INVSIZE_Y) + INVSIZE_Y);
	
	vector<nlohmann::json> itemJsons = jsonInv["items"];
	for(auto& itemJson : itemJsons) {
		items.push_back(InventoryItem::Deserialize(itemJson));
	}


	

	InvokeOnChange();
	InvokeOnSelect();
}

void Inventory::LoadDefaultItems() {
	items.clear();
	AddItem(COPPER_PICKAXE);
	AddItem(COPPER_AXE);
	AddItem(COPPER_SHOVEL);
	AddItem(WORKBENCH, 5);
	AddItem(APPLE, 5);
	AddItem(RAW_STEAK, 5);
	InvokeOnChange();
	InvokeOnSelect();
}

void Inventory::DropAllItems(Vector3 position)
{
	//InventoryItem* newDroppedItem = new InventoryItem(heldItem->type, heldItem->ID, -1, -1, 1);
	//DroppedItem* droppedItemEntity = DroppedItem::Create(newDroppedItem, transform.position + (transform.forward() * 1.46f));

	for(auto& inventoryItem : items) {
		DroppedItem* droppedItemEntity = DroppedItem::Create(inventoryItem, position);
	}
	items.clear();
}

InventoryItem* Inventory::GetItemAt(const int x, const int y) {
	for(InventoryItem* invItem : items) {
		if(invItem->posX == x && invItem->posY == y) {
			return invItem;
		}
	}
	return nullptr;
}

void Inventory::SetSlotNum(const int num) {
	selectedSlotNum = num;
	InvokeOnSelect();
}

void Inventory::ChangeSlotNum(const int delta) {
	selectedSlotNum += delta;
	selectedSlotNum = FloorMod(selectedSlotNum, INVSIZE_X);
	InvokeOnSelect();
}

void Inventory::AddOnChangeEvent(function<void()> func) {
	_onChangeEvents.push_back(func);
}

void Inventory::AddOnSelectEvent(function<void(int)> func) {
	_onSelectEvents.push_back(func);
}

void Inventory::InvokeOnChange() {
	for(auto& func : _onChangeEvents) {
		func();
	}
}

void Inventory::InvokeOnSelect() {
	for(auto& func : _onSelectEvents) {
		func(selectedSlotNum);
	}
}