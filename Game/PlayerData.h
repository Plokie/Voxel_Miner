#pragma once

#include "../Engine/json.hpp"
#include "../Engine/MathUtil.h"
#include "../Engine/Object3D.h"

enum HUNGER_DECREMENT_STATE : unsigned char { HDS_WALK, HDS_SPRINT, HDS_JUMP, HDS_COUNT };
enum DAMAGE_STATE : unsigned char { DS_LAVA, DS_COUNT };

#define HEALTH_MAX 20
#define HUNGER_MAX 20
#define OXYGEN_MAX 20

class Inventory;

enum GAMEMODE {
	GM_SURVIVAL,
	GM_CREATIVE
};

class PlayerData : public Object3D {
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


	// Data relevant to run-time
	Transform* _pInstantiatedPlayerTransform = nullptr;

	// Data to-be serialized:
	Vector3 _storedPosition; // Not necessarily the actual current position, but the most recent stored position
	Inventory* _pInventory;
	string _name;
	GAMEMODE _gamemode = GM_SURVIVAL;



	// Update / mechanical stuff
	vector<function<void()>> _onDeathEvents;

	void InvokeOnDeathEvent() {
		for(auto& func : _onDeathEvents) {
			func();
		}
	}


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
	int saturation = HUNGER_MAX / 2;
	vector<function<void(int)>> _onHungerChangeEvents;
	void InvokeOnHungerChange() {
		for(auto& func : _onHungerChangeEvents) {
			func(hunger);
		}
	}

	int oxygen = OXYGEN_MAX;
	vector<function<void(int)>> _onOxygenChangeEvents;
	void InvokeOnOxygenChange() {
		for(auto& func : _onOxygenChangeEvents) {
			func(oxygen);
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


	// inventory events
	/*vector<function<void()>> _onItemChangeEvents;
	vector<function<void(int)>> _onItemSelectEvents;*/

	//void InvokeOnItemChange();
	//void InvokeOnItemSelect();

public:
	bool isSuffocating = false;
	/*void AddOnChangeEvent(function<void()> func);
	void AddOnSelectEvent(function<void(int)> func);*/


	GAMEMODE GetGamemode() const { return _gamemode; }
	void SetGamemode(GAMEMODE gm);


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
	void ChangeHunger(int amt, bool saturate = true) {
		hunger += amt;
		hunger = min(hunger, HUNGER_MAX);
		if(saturate) {
			saturation += amt;
			saturation = min(saturation, HUNGER_MAX / 2);
		}
		InvokeOnHungerChange();
	}
	void AddOnHungerChangeEvent(function<void(int)> func) {
		_onHungerChangeEvents.emplace_back(func);
	}

	void SetOxygen(int amt) { oxygen = amt; InvokeOnOxygenChange(); }
	const int GetOxygen() const { return oxygen; }
	void ChangeOxygen(int amt) {
		oxygen += amt;
		oxygen = max(0, min(oxygen, OXYGEN_MAX));
		InvokeOnOxygenChange();
	}
	void AddOnOxygenChangeEvent(function<void(int)> func) {
		_onOxygenChangeEvents.emplace_back(func);
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

	bool IsCreative() {
		return _gamemode == GM_CREATIVE;
	}

	PlayerData();
	//PlayerData(string playerName);

	Inventory* GetInventory() { return _pInventory; }


	// Only to be called if its the currently active player
	//void PlayerUpdate(const float dt);
	void Update(const float dt) override;

	nlohmann::json Serialize();
	void Deserialize(nlohmann::json json);

};