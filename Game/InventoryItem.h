#pragma once

#include "../Engine/MathUtil.h"
#include "Items.h"
#include "Blocks.h"

struct InventoryItem {
public:
	enum Type : unsigned short { ITEM, BLOCK };
private:

public:
	InventoryItem::Type type = ITEM;
	unsigned short ID = 0; // Could be block or item
	int posX, posY;

	InventoryItem(InventoryItem::Type type, unsigned short ID, int posX, int posY) :
		type(type), ID(ID), posX(posX), posY(posY) {}

	InventoryItem(BlockID blockID, int posX, int posY) :
		type(Type::BLOCK), ID(blockID), posX(posX), posY(posY) {}

	InventoryItem(ItemID itemID, int posX, int posY) :
		type(Type::ITEM), ID(itemID), posX(posX), posY(posY) {}

	Vector2Int GetUVPos();
};