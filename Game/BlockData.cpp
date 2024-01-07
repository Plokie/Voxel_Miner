#include "BlockData.h"

#include "Inventory.h"

nlohmann::json BlockData::Serialize()
{
	nlohmann::json json = {};

	json["timer"] = timer;
	if(blockInventory == nullptr) {
		json["inventory"] = 0;
	}
	else {
		json["inventory"] = blockInventory->Serialize();
	}

	return json;
}

void BlockData::Deserialize(nlohmann::json json) {
	timer = json["timer"];

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
