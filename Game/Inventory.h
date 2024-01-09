#pragma once

#include <functional>

#include "../Engine/Object3D.h"
#include "InventoryItem.h"
#include "Blocks.h"
#include "Items.h"

#define INVSIZE_X 9
#define INVSIZE_Y 5 // Includes hotbar

enum HUNGER_DECREMENT_STATE : unsigned char { HDS_WALK, HDS_SPRINT, HDS_JUMP, HDS_COUNT };
enum DAMAGE_STATE : unsigned char { DS_LAVA, DS_COUNT };

#define HEALTH_MAX 20
#define HUNGER_MAX 20

class InventoryUI;
class Recipe;

// Does not exist in space, just holds data
class Inventory : public Object3D {
private:
	map<HUNGER_DECREMENT_STATE, bool> _hungerDecrementStates = {
		{HDS_WALK, false},
		{HDS_SPRINT, false},
		{HDS_JUMP, false},
	};
	static map<HUNGER_DECREMENT_STATE, float> _hungerDecrementValues;

	map<DAMAGE_STATE, bool> _damageStates = {
		{DS_LAVA, false}
	};


	InventoryItem errorInvItem = InventoryItem(InventoryItem::Type::BLOCK, 0, -1, -1, 0);
	vector<InventoryItem*> items;
	map<tuple<int, int>, InventoryItem*> _itemPosMap;

	// return {-1,-1} if no spots are free
	const bool DoesItemExistAtPos(int posX, int posY) const;

	vector<function<void()>> _onChangeEvents;
	vector<function<void(int)>> _onSelectEvents;

	vector<function<void()>> _onDeathEvents;

	void InvokeOnDeathEvent() {
		for(auto& func : _onDeathEvents) {
			func();
		}
	}

	int selectedSlotNum = 0;

	void InvokeOnChange();
	void InvokeOnSelect();
	
	const float secondsPerDamageTick = 0.8f;
	float damageTickTimer = 3.f;
	int health = HEALTH_MAX;
	vector<function<void(int)>> _onHealthChangeEvents;
	void InvokeOnHealthChange() {
		for(auto& func : _onHealthChangeEvents) {
			func(health);
		}
	}

	int hunger = HUNGER_MAX;
	int saturation = HUNGER_MAX/2;
	vector<function<void(int)>> _onHungerChangeEvents;
	void InvokeOnHungerChange() {
		for(auto& func : _onHungerChangeEvents) {
			func(hunger);
		}
	}


	int score = 0; // Gonna remove this later, just need it for competencies
	vector<function<void(int)>> _onScoreChangeEvents;
	void InvokeOnScoreChange() {
		for(auto& func : _onScoreChangeEvents) {
			func(score);
		}
	}

	float hungerDecrementer = 0.f;
public:
	// Gonna be removing these later
	void SetScore(int amt) { score = amt; InvokeOnScoreChange(); }
	const int GetScore() const { return score; }
	void ChangeScore(int amt) { score += amt; InvokeOnScoreChange(); }
	void AddOnScoreChangeEvent(function<void(int)> func) {
		_onScoreChangeEvents.emplace_back(func);
	}

	const int GetSaturation() const { return saturation; }

	void SetHealth(int amt) { health = amt; InvokeOnHealthChange(); }
	const int GetHealth() const { return health; }
	void ChangeHealth(int amt);
	void AddOnHealthChangeEvent(function<void(int)> func) {
		_onHealthChangeEvents.emplace_back(func);
	}

	void SetHunger(int amt) { hunger = amt; InvokeOnHungerChange(); }
	const int GetHunger() const { return hunger; }
	void ChangeHunger(int amt, bool saturate=true) { 
		hunger += amt; 
		hunger = min(hunger, HUNGER_MAX);
		if(saturate) {
			saturation += amt;
			saturation = min(saturation, HUNGER_MAX/2);
		}
		InvokeOnHungerChange();
	}
	void AddOnHungerChangeEvent(function<void(int)> func) {
		_onHungerChangeEvents.emplace_back(func);
	}

	void SetHungerFlag(HUNGER_DECREMENT_STATE flag, bool state) { _hungerDecrementStates[flag] = state; }
	void DecrementHungerFlag(HUNGER_DECREMENT_STATE flag) {
		hungerDecrementer += _hungerDecrementValues[flag];
	}

	void SetDamageFlag(DAMAGE_STATE flag, bool state) {
		_damageStates[flag] = state;
	}

	void AddOnDeathEvent(function<void()> func) {
		_onDeathEvents.emplace_back(func);
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
	bool AddItem(const InventoryItem* item, InventoryItem** outInvItem = nullptr);
	bool AddItem(const BlockID blockID, const int amount = 1, InventoryItem** outInvItem = nullptr);
	bool AddItem(const ItemID itemID, const int amount = 1, InventoryItem** outInvItem = nullptr);
	bool AddItem(const unsigned int ID, const InventoryItem::Type type, const int amount = 1, InventoryItem** outInvItem = nullptr);

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

	void DropAllItems(Vector3 position);

	void Update(float dt) override;
};