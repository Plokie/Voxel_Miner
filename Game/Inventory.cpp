#include "Inventory.h"

#include "Crafting.h"

const Vector2Int Inventory::GetFreeSpot() const {
	Vector2Int tryPos = { 0, 0 };

	while(DoesItemExistAtPos(tryPos.x, tryPos.y)) {
		tryPos += Vector2Int(1, 0);

		if(tryPos.x >= INVSIZE_X) {
			tryPos.x = 0;
			tryPos.y += 1;
		}
	} 

	return tryPos;
}

bool Inventory::GetHeldItem(InventoryItem** out)
{
	for(InventoryItem& invItem : items) {
		if(invItem.posX == selectedSlotNum && invItem.posY == 0) {
			*out = &invItem;
			return true;
		}
	}
	return false;
}

void Inventory::AddItem(const InventoryItem& item) {
	if(item.type == InventoryItem::Type::BLOCK) {
		AddItem((BlockID)item.ID, item.amount);
	}
	else {
		AddItem((ItemID)item.ID, item.amount);
	}
}

void Inventory::AddItem(const BlockID blockID, const int amount) {
	AddItem(blockID, InventoryItem::Type::BLOCK, amount);
}

const bool Inventory::DoesItemExistAtPos(int posX, int posY) const
{
	for(const auto& invItem : items) {
		if(invItem.posX == posX && invItem.posY == posY) return true;
	}
	return false;
}

void Inventory::AddItem(const ItemID itemID, const int amount) {
	AddItem(itemID, InventoryItem::Type::ITEM, amount);
}

void Inventory::AddItem(const unsigned int ID, const InventoryItem::Type type, const int amount) {
	for(auto& invItem : items) {
		int maxStack = (invItem.type == InventoryItem::Type::ITEM) ? ItemDef::Get((ItemID)invItem.ID).GetMaxStack() : MAX_STACK;
		if(invItem.ID == ID && invItem.type == type && invItem.amount < maxStack) {
			invItem.amount += amount;

			if(invItem.amount > maxStack) {
				AddItem(ID, type, invItem.amount - amount);
				invItem.amount = maxStack;
			}
			else 
			{
				InvokeOnChange();
				InvokeOnSelect();
			}
			return;
		}
	}



	// need to split stack here
	int maxStack = (type == InventoryItem::Type::ITEM) ? ItemDef::Get((ItemID)ID).GetMaxStack() : MAX_STACK;

	if(amount <= maxStack) {
		Vector2Int newPos = GetFreeSpot();
		items.emplace_back(type, ID, newPos.x, newPos.y, amount);
	}
	else {
		int fullstacks = amount / maxStack;
		int remainder = amount % maxStack;

		for(int i = 0; i < fullstacks; i++) {
			Vector2Int newPos = GetFreeSpot();
			items.emplace_back(type, ID, newPos.x, newPos.y, maxStack);
		}

		if(remainder > 0) {
			Vector2Int newPos = GetFreeSpot();
			items.emplace_back(type, ID, newPos.x, newPos.y, remainder);
		}
	}

	
	
	
	InvokeOnChange();
	InvokeOnSelect();
}

void Inventory::SubItem(const BlockID blockID, const int amount) {
	SubItem(blockID, InventoryItem::Type::BLOCK, amount);
}

void Inventory::SubItem(const ItemID itemID, const int amount) {
	SubItem(itemID, InventoryItem::Type::ITEM, amount);
}

void Inventory::SubItem(const unsigned int ID, const InventoryItem::Type type, const int amount) {
	for(auto it = items.begin(); it != items.end(); it++) {
		InventoryItem& invItem = *it;
		if(invItem.ID == ID && invItem.type == type) {

			//todo: if sub more than a stack, sub remainder too
			invItem.amount -= amount;
			if(invItem.amount <= 0) {
				items.erase(it);
			}
			InvokeOnChange();
			InvokeOnSelect();
			return;
		}
	}
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
	for(const InventoryItem& item : items) {
		if(item.ID == ID && item.type == type) count += item.amount;
	}

	return count;
}

void Inventory::ClearEmptyItems() {
	/*for(auto it = items.begin(); it != items.end();) {
		if(it->amount <= 0) {
			items.erase(it);
		}
		else {
			it++;
		}
	}*/
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
		itemJsons.push_back(invItem.Serialize());
	}
	json["items"] = itemJsons;
	json["score"] = score;
	//json[""]

	return json;
}

void Inventory::Deserialize(nlohmann::json jsonInv) {
	items.clear();
	
	vector<nlohmann::json> itemJsons = jsonInv["items"];
	for(auto& itemJson : itemJsons) {
		items.push_back(InventoryItem::Deserialize(itemJson));
	}


	if(jsonInv.find("score")!=jsonInv.end())
		score = jsonInv["score"];

	InvokeOnChange();
	InvokeOnSelect();
	InvokeOnScoreChange();
}

void Inventory::LoadDefaultItems() {
	items.clear();
	AddItem((ItemID)COPPER_PICKAXE);
	AddItem((ItemID)COPPER_AXE);
	AddItem((ItemID)COPPER_SHOVEL);
	InvokeOnChange();
	InvokeOnSelect();
}

InventoryItem& Inventory::GetItemAt(const int x, const int y) {
	for(InventoryItem& invItem : items) {
		if(invItem.posX == x && invItem.posY == y) {
			return invItem;
		}
	}
	return errorInvItem;
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