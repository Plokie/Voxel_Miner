#pragma once

#include "Entity.h"
//#include "InventoryItem.h"
struct InventoryItem;

class DroppedItem : public Entity {
private:
	InventoryItem* invItem = nullptr;
public:

	void SetupModel();
	void OnCollide(PlayerController* pc) override;

	//Destroys the DroppedItem as well as its attached invItem
	void Annihilate(); // Needed something more descriptively final. ANNIHILATE sounds cool

	DroppedItem(InventoryItem* invItem);

	static DroppedItem* Create(InventoryItem* invItem, Vector3 worldPosition);

	void Start() override;
	void Update(float dt) override;
};