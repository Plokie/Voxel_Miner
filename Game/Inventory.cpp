#include "Inventory.h"

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
		int maxStack = (invItem.type == InventoryItem::Type::ITEM) ? ItemDef::Get((ItemID)invItem.ID).GetMaxStack() : 64;
		if(invItem.ID == ID && invItem.type == type && invItem.amount < maxStack) {
			invItem.amount += amount;

			if(invItem.amount > maxStack) {
				AddItem(ID, type, invItem.amount - amount);
				invItem.amount = maxStack;
			}
			else InvokeOnChange();
			return;
		}
	}



	// need to split stack here
	int maxStack = (type == InventoryItem::Type::ITEM) ? ItemDef::Get((ItemID)ID).GetMaxStack() : 64;

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
}

void Inventory::AddOnChangeEvent(function<void()> func) {
	_onChangeEvents.push_back(func);
}

void Inventory::InvokeOnChange() {
	for(auto& func : _onChangeEvents) {
		func();
	}
}