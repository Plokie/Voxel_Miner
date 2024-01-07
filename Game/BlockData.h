#pragma once

#include "../Engine/json.hpp"

class Inventory;

class BlockData {
private:
public:
	float timer = 0.f;
	Inventory* blockInventory = nullptr;

	BlockData() {}
	BlockData(float timer) : timer(timer){}
	BlockData(Inventory* blockInventory) : blockInventory(blockInventory) {}

	nlohmann::json Serialize();
	void Deserialize(nlohmann::json json);

	~BlockData();
};