#pragma once

#include "../Engine/json.hpp"
#include "../Engine/MathUtil.h"

class Inventory;

class BlockData {
private:
public:
	float timer1 = 0.f;
	float timer2 = 0.f;
	Inventory* blockInventory = nullptr;

	BlockData() {}
	BlockData(float timer1) : timer1(timer1){}
	BlockData(float timer1, float timer2) : timer1(timer1), timer2(timer2){}
	BlockData(Inventory* blockInventory) : blockInventory(blockInventory) {}

	void TryDropItems(Vector3 worldPosition);

	nlohmann::json Serialize();
	void Deserialize(nlohmann::json json);

	~BlockData();
};