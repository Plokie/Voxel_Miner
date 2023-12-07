#pragma once

#include "../Engine/Object3D.h"
#include "Items.h"
#include "Blocks.h"
#include "InventoryItem.h"


class HeldItem : public Object3D {
private:

public:
	HeldItem();

	void Start() override;

	void SetItem(BlockID blockID);
	void SetItem(ItemID itemID);
	void SetItem(InventoryItem::Type type, unsigned short id);
};