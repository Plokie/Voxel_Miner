#pragma once

#include <map>
#include <string>
#include "ItemTypes.h"

#define ITEM_ATLAS_SIZE 256
#define ITEM_ATLAS_TILE_SIZE 16

using namespace std;

enum ItemID : unsigned short {
	ITEMERR, COAL, RAW_COPPER, RAW_GOLD, AMETHYST, RAW_TITANIUM,
	COPPER_BAR, GOLD_BAR, TITANIUM_BAR,
	COPPER_PICKAXE, GOLD_PICKAXE, AMETHYST_PICKAXE, TITANIUM_PICKAXE,
	COPPER_AXE, GOLD_AXE, AMETHYST_AXE, TITANIUM_AXE,
	COPPER_SHOVEL, GOLD_SHOVEL, AMETHYST_SHOVEL, TITANIUM_SHOVEL,
	STICK
};

class Item {
private:
	string name;

	int uvX, uvY;
	int maxStack;
	ItemType itemType = ItemType::BASICITEM;
public:
	Item(string name, int uvX, int uvY, int maxStack=64, ItemType itemType = ItemType::BASICITEM) : name(name), uvX(uvX), uvY(uvY), maxStack(maxStack), itemType(itemType) {}

	const string& GetName() const { return name; }
	const int UVx() const { return uvX; }
	const int UVy() const { return uvY; }
	const int GetMaxStack() const { return maxStack; }
	const ItemType GetItemType() const { return itemType; }
};

class ItemDef {
private:
	static const map<ItemID, Item> defs;
public:
	static const Item& Get(ItemID id);
};