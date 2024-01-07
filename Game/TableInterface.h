#pragma once

#include <vector>
#include "../Engine/Object2D.h"
#include "Blocks.h"
//#include "../Engine/UI/Button.h"
//#include "../Engine/UI/HorizontalLayoutRect.h"

class HorizontalLayoutRect;
class Button;
class UIRect;
class Inventory;
class ItemIcon;

struct InterfaceContext {
	Inventory* playerInventory;
	Inventory* tableInventory;
	Vector3Int tablePosition;
	BlockID blockID = BlockID::AIR;

	InterfaceContext(Inventory* playerInventory, Inventory* tableInventory, Vector3Int tablePosition, BlockID blockID) {
		this->playerInventory = playerInventory;
		this->tableInventory = tableInventory;
		this->tablePosition = tablePosition;
		this->blockID = blockID;
	}

	InterfaceContext(Inventory* playerInventory, BlockID blockID) {
		this->playerInventory = playerInventory;
		this->tableInventory = nullptr;
		this->tablePosition = Vector3Int(0,0,0);
		this->blockID = blockID;
	}
};

class TableInterface : public Object2D {
protected:
	vector<Button*> slots;
	vector<ItemIcon*> _spawnedItemIcons;
public:

	virtual void Open(InterfaceContext ctx) = 0;
	virtual void Close();

	void Start() override;
	void Update(const float dt) override = 0;
	void Draw(SpriteBatch* sb) override = 0;
};