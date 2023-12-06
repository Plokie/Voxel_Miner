#pragma once

#include <functional>

#include "../Engine/Object3D.h"
#include "InventoryItem.h"
#include "Blocks.h"
#include "Items.h"

#define INVSIZE_X 9
#define INVSIZE_Y 4 // Includes hotbar

class InventoryUI;

// Does not exist in space, just holds data
class Inventory : public Object3D {
private:
	vector<InventoryItem> items;
	map<tuple<int, int>, InventoryItem*> _itemPosMap;

	// return {-1,-1} if no spots are free
	const Vector2Int GetFreeSpot() const;
	const bool DoesItemExistAtPos(int posX, int posY) const;

	vector<function<void()>> _onChangeEvents;

	void InvokeOnChange();
public:

	void AddOnChangeEvent(function<void()> func);

	const vector<InventoryItem>& GetInventoryItems() { return items; }

	void AddItem(const InventoryItem& item);
	void AddItem(const BlockID blockID, const int amount = 1);
	void AddItem(const ItemID itemID, const int amount = 1);
	void AddItem(const unsigned int ID, const InventoryItem::Type type, const int amount = 1);
};