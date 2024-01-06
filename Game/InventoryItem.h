#pragma once

#include "../Engine/MathUtil.h"
#include "Items.h"
#include "Blocks.h"
#include "../Engine/json.hpp"

struct InventoryItem {
public:
	enum Type : unsigned short { ITEM, BLOCK };
private:

public:
	int amount = 1;
	InventoryItem::Type type = ITEM;
	unsigned short ID = 0; // Could be block or item
	int posX=0, posY=0;

	InventoryItem() {}

	InventoryItem(InventoryItem::Type type, unsigned short ID, int posX, int posY, int amount = 1) :
		type(type), ID(ID), posX(posX), posY(posY), amount(amount) {}

	InventoryItem(BlockID blockID, int posX, int posY, int amount = 1) :
		type(Type::BLOCK), ID(blockID), posX(posX), posY(posY), amount(amount) {}

	InventoryItem(ItemID itemID, int posX, int posY, int amount = 1) :
		type(Type::ITEM), ID(itemID), posX(posX), posY(posY), amount(amount) {}

	Vector2Int GetUVPos(BLOCK_FACE_TEX face_tex = BLOCK_FACE_TEX::FT_SIDE) const;

	const int GetMaxStack();

	bool IsSameItemAs(const InventoryItem& other);
	bool IsSameItemAs(const InventoryItem* other);
	bool Is(BlockID blockID);
	bool Is(ItemID itemID);

	nlohmann::json Serialize();
	static InventoryItem* Deserialize(nlohmann::json json);
};