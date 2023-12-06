#pragma once

#include "../Engine/Object3D.h"
#include "InventoryItem.h"
#include "Blocks.h"
#include "Items.h"

// Does not exist in space, just holds data
class Inventory : public Object3D {
private:
	vector<InventoryItem> items;


public:

	void AddItem(const InventoryItem& item, const int amount = 1);
	void AddItem(const BlockID blockID, const int amount = 1);
	void AddItem(const ItemID itemID, const int amount = 1);

};