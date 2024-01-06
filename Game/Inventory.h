#pragma once

#include <functional>

#include "../Engine/Object3D.h"
#include "InventoryItem.h"
#include "Blocks.h"
#include "Items.h"

#define INVSIZE_X 9
#define INVSIZE_Y 5 // Includes hotbar

class InventoryUI;
class Recipe;

// Does not exist in space, just holds data
class Inventory : public Object3D {
private:
	InventoryItem errorInvItem = InventoryItem(InventoryItem::Type::BLOCK, 0, -1, -1, 0);
	vector<InventoryItem*> items;
	map<tuple<int, int>, InventoryItem*> _itemPosMap;

	// return {-1,-1} if no spots are free
	const bool DoesItemExistAtPos(int posX, int posY) const;

	vector<function<void()>> _onChangeEvents;
	vector<function<void(int)>> _onSelectEvents;

	int selectedSlotNum = 0;

	void InvokeOnChange();
	void InvokeOnSelect();
	
	int score = 0; // Gonna remove this later, just need it for competencies
	vector<function<void(int)>> _onScoreChangeEvents;
	void InvokeOnScoreChange() {
		for(auto& func : _onScoreChangeEvents) {
			func(score);
		}
	}
public:
	// Gonna be removing these later
	void SetScore(int amt) { score = amt; InvokeOnScoreChange(); }
	const int GetSore() const { return score; }
	void ChangeScore(int amt) { score += amt; InvokeOnScoreChange(); }
	void AddOnScoreChangeEvent(function<void(int)> func) {
		_onScoreChangeEvents.emplace_back(func);
	}

	const Vector2Int GetFreeSpot() const;

	bool GetHeldItem(InventoryItem** out);
	bool GetItemAt(int x, int y, InventoryItem** out);

	void SetSlotNum(const int num);
	void ChangeSlotNum(const int delta);
	void AddOnChangeEvent(function<void()> func);
	void AddOnSelectEvent(function<void(int)> func);
	_NODISCARD InventoryItem* GetItemAt(const int x, const int y);

	vector<InventoryItem*>& GetInventoryItems() { return items; } // bad

	// Dont forget to delete InventoryItem* instance if desired
	bool AddItem(const InventoryItem* item, int* outRemainder = nullptr);
	bool AddItem(const BlockID blockID, const int amount = 1, int* outRemainder = nullptr);
	bool AddItem(const ItemID itemID, const int amount = 1, int* outRemainder = nullptr);
	bool AddItem(const unsigned int ID, const InventoryItem::Type type, const int amount = 1, int* outRemainder = nullptr);

	void PushItem(InventoryItem* item);

	void SubItem(const BlockID blockID, const int amount = 1);
	void SubItem(const ItemID itemID, const int amount = 1);
	void SubItem(const unsigned int ID, const InventoryItem::Type type, const int amount = 1);

	void SubHeldItem(int amount = 1, InventoryItem* precalculatedInvItem = nullptr);

	int GetItemCount(const BlockID blockID);
	int GetItemCount(const ItemID itemID);
	int GetItemCount(const unsigned int ID, const InventoryItem::Type type);

	void ClearEmptyItems();

	bool CanCraft(const Recipe& recipe);
	bool TryCraft(const Recipe& recipe);

	nlohmann::json Serialize();
	void Deserialize(nlohmann::json jsonInv);
	void LoadDefaultItems();
};