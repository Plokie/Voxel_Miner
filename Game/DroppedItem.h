#pragma once

#include "Entity.h"
//#include "InventoryItem.h"
class InventoryItem;

class DroppedItem : public Entity {
private:
	InventoryItem* invItem = nullptr;
public:

	void SetupModel();
	void OnCollide(PlayerController* pc) override;

	//Destroys the DroppedItem as well as its attached invItem
	void Annihilate(); // Needed something more descriptively final. ANNIHILATE sounds cool

	DroppedItem(InventoryItem* invItem);

	void Start() override;
	void Update(float dt) override;
};