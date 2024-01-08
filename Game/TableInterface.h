#pragma once

#include <vector>
#include "../Engine/Object2D.h"
#include "Blocks.h"
//#include "../Engine/UI/Button.h"
//#include "../Engine/UI/HorizontalLayoutRect.h"
#include "InventoryItem.h"

class HorizontalLayoutRect;
class Button;
class UIRect;
class Inventory;
class InventoryUI;
class ItemIcon;
class BlockData;

struct InterfaceContext {
	Inventory* playerInventory;
	BlockData* blockData;
	Vector3Int tablePosition;
	BlockID blockID = BlockID::AIR;

	InterfaceContext(Inventory* playerInventory, BlockData* blockData, Vector3Int tablePosition, BlockID blockID) {
		this->playerInventory = playerInventory;
		this->blockData = blockData;
		this->tablePosition = tablePosition;
		this->blockID = blockID;
	}

	InterfaceContext(Inventory* playerInventory, BlockID blockID) {
		this->playerInventory = playerInventory;
		this->blockData = nullptr;
		this->tablePosition = Vector3Int(0,0,0);
		this->blockID = blockID;
	}
};

class TableInterface : public Object2D {
protected:
	vector<Button*> _spawnedSlots;
	vector<ItemIcon*> _spawnedItemIcons;
	vector<UIRect*> _spawnedUIRects;
	BlockData* blockData;
	InventoryUI* invUI;
	map<tuple<int, int>, InventoryItem*> itemExistsHash;

	map<tuple<int, int>, Button*> _indexedSlots;

	Button* MakeSlot(int idx, int idy);
	ItemIcon* MakeItemIcon(InventoryItem* invItem);
	void ComputeItemExistsHash();

	// If an item exists at position idx,idy. Create it as child of parent.
	bool TryMakeItemIcon(Button* parent, int idx, int idy);
public:
	void ReleaseHeldItem();
	bool EraseIcon(ItemIcon* icon);
	void ReloadIcons();
	void PushIcon(ItemIcon* itemIcon);

	virtual void Open(InterfaceContext ctx);
	virtual void Close();

	void Start() override = 0;
	void Update(const float dt) override;
	void Draw(SpriteBatch* sb) override;
};