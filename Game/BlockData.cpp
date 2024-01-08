#include "BlockData.h"

#include "Inventory.h"

void BlockData::TryDropItems(Vector3 worldPosition)
{
	if(blockInventory) {
		blockInventory->DropAllItems(worldPosition);
		delete blockInventory;
		blockInventory = nullptr;
	}
}

nlohmann::json BlockData::Serialize()
{
	nlohmann::json json = {};

	json["timer1"] = timer1;
	json["timer2"] = timer2;
	if(blockInventory == nullptr) {
		json["inventory"] = 0;
	}
	else {
		json["inventory"] = blockInventory->Serialize();
	}

	return json;
}

void BlockData::Deserialize(nlohmann::json json) {
	timer1 = json["timer1"];
	timer2 = json["timer2"];

	if(json["inventory"] == 0) {
		blockInventory = nullptr;
	}
	else {
		blockInventory = new Inventory();
		blockInventory->Deserialize(json["inventory"]);
	}
}

BlockData::~BlockData() {
	if(blockInventory) delete blockInventory;
}
