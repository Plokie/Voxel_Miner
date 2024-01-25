#include "PlayerData.h"

#include "../Engine/Engine.h"
#include "Inventory.h"
#include "../Audio/Audio.h"

map<HUNGER_DECREMENT_STATE, float> PlayerData::_hungerDecrementValues = {
	{HDS_WALK, 0.000001f},
	{HDS_SPRINT, 0.000001f},
	{HDS_JUMP, 1.0f},
};


void PlayerData::SetGamemode(GAMEMODE gm)
{
	_gamemode = gm;
}

void PlayerData::ChangeHealth(int amt) {
	health += amt;
	InvokeOnHealthChange();
	if(amt < 0) {
		Audio::Play("hurt", 1.f);
	}

	if(health <= 0) {
		//DropAllItems();
		health = HEALTH_MAX;
		saturation = HEALTH_MAX / 2;
		hunger = HUNGER_MAX;
		InvokeOnHealthChange();
		InvokeOnHungerChange();

		score /= 2;
		InvokeOnScoreChange();

		InvokeOnDeathEvent();
	}
}

PlayerData::PlayerData() {
	_pInventory = new Inventory();
}

//PlayerData::PlayerData(string playerName)
//{
//	// load player data from file
//}

void PlayerData::Update(const float dt) {
	if(IsCreative()) return;

	float currentExhaustion = 0.0f;

	for(int i = 0; i < HDS_COUNT; i++) {
		currentExhaustion += (float)_hungerDecrementStates[(HUNGER_DECREMENT_STATE)i] * _hungerDecrementStates[(HUNGER_DECREMENT_STATE)i];
	}



	hungerDecrementer += dt * currentExhaustion;

	if(hungerDecrementer > 25.f) {
		hungerDecrementer = 0.f;

		if(saturation > 0) {
			saturation--;
		}
		else {
			hunger--;
			InvokeOnHungerChange();
		}
	}


	damageTickTimer -= dt;
	if(damageTickTimer <= 0.0f) {
		damageTickTimer = secondsPerDamageTick;

		// check damage state and hurt
		bool isDamaging = false;
		for(int i = 0; i < DS_COUNT; i++) {
			if(_damageStates[(DAMAGE_STATE)i]) {
				isDamaging = true;
				break;
			}
		}
		if(oxygen <= 0) isDamaging = true;

		if(isDamaging) {
			ChangeHealth(-2);
		}

		if(isSuffocating) {
			ChangeOxygen(-1);
		}
		else {
			ChangeOxygen(2);
		}

		if(health < HEALTH_MAX && saturation > 0) {
			saturation--;
			ChangeHealth(1);
		}

		if(hunger <= 0 && saturation <= 0) {
			ChangeHealth(-1);
		}
	}
}

nlohmann::json PlayerData::Serialize()
{
	nlohmann::json json = {};
	
	Vector3 playerPos = Engine::Get()->GetScene("game")->GetObject3D("PlayerController")->transform.position;

	json["position"] = { playerPos.x, playerPos.y, playerPos.z };
	json["score"] = score;
	json["health"] = health;
	json["hunger"] = hunger;
	json["saturation"] = saturation;
	json["gamemode"] = _gamemode;
	json["oxygen"] = oxygen;
	
	if(_pInventory) {
		json["inventory"] = _pInventory->Serialize();
	}
	else {
		json["inventory"] = 0;
	}
	
	
	return json;
}

void PlayerData::Deserialize(nlohmann::json json) {

	Engine::Get()->GetScene("game")->GetObject3D("PlayerController")->transform.position = { json["position"][0], json["position"][1], json["position"][2] };

	if(json.find("score") != json.end())
		score = json["score"];

	if(json.find("health") != json.end())
		health = json["health"];

	if(json.find("hunger") != json.end())
		hunger = json["hunger"];

	if(json.find("saturation") != json.end())
		saturation = json["saturation"];
	
	if(json.find("gamemode") != json.end())
		_gamemode = (GAMEMODE)json["gamemode"];
	
	if(json.find("oxygen") != json.end())
		oxygen = json["oxygen"];

	if(json.find("inventory") != json.end()) {
		//if(json["inventory"] != 0) {
		//if(_pInventory) delete _pInventory;

		if(!_pInventory) _pInventory = new Inventory();

		//_pInventory = new Inventory();
		_pInventory->Deserialize(json["inventory"]);
		//}
	}

	InvokeOnScoreChange();
	InvokeOnHungerChange();
	InvokeOnHealthChange();
}
