#pragma once

#include <functional>

#include "../Engine/Object3D.h"
#include "InventoryItem.h"
#include "Blocks.h"
#include "Items.h"

#define INVSIZE_X 9
#define INVSIZE_Y 5 // Includes hotbar

class InventoryUI;

// Does not exist in space, just holds data
class Inventory : public Object3D {
private:
	InventoryItem errorInvItem = InventoryItem(InventoryItem::Type::BLOCK, 0, -1, -1, 0);
	vector<InventoryItem> items;
	map<tuple<int, int>, InventoryItem*> _itemPosMap;

	// return {-1,-1} if no spots are free
	const bool DoesItemExistAtPos(int posX, int posY) const;

	vector<function<void()>> _onChangeEvents;
	vector<function<void(int)>> _onSelectEvents;

	int selectedSlotNum = 0;

	void InvokeOnChange();
	void InvokeOnSelect();
public:
	const Vector2Int GetFreeSpot() const;

	bool GetHeldItem(InventoryItem** out);

	void SetSlotNum(const int num);
	void ChangeSlotNum(const int delta);
	void AddOnChangeEvent(function<void()> func);
	void AddOnSelectEvent(function<void(int)> func);
	_NODISCARD InventoryItem& GetItemAt(const int x, const int y);

	vector<InventoryItem>& GetInventoryItems() { return items; }

	void AddItem(const InventoryItem& item);
	void AddItem(const BlockID blockID, const int amount = 1);
	void AddItem(const ItemID itemID, const int amount = 1);
	void AddItem(const unsigned int ID, const InventoryItem::Type type, const int amount = 1);

	void SubItem(const BlockID blockID, const int amount = 1);
	void SubItem(const ItemID itemID, const int amount = 1);
	void SubItem(const unsigned int ID, const InventoryItem::Type type, const int amount = 1);

	nlohmann::json Serialize();
	void Deserialize(nlohmann::json jsonInv);
	void LoadDefaultItems();
};