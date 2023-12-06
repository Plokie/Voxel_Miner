#pragma once

#include <map>
#include <string>

#define ITEM_ATLAS_SIZE 256
#define ITEM_ATLAS_TILE_SIZE 16

using namespace std;

enum ItemID : unsigned short {
	ITEMERR, COAL, RAW_COPPER, RAW_GOLD, AMETHYST, RAW_TITANIUM,
	COPPER_BAR, GOLD_BAR, TITANIUM_BAR,
	COPPER_PICKAXE, GOLD_PICKAXE, AMETHYST_PICKAXE, TITANIUM_PICKAXE,
	COPPER_AXE, GOLD_AXE, AMETHYST_AXE, TITANIUM_AXE,
};

class Item {
private:
	string name;

	int uvX, uvY;
public:
	Item(string name, int uvX, int uvY) : name(name), uvX(uvX), uvY(uvY) {}

	const string& GetName() const { return name; }
	const int UVx() const { return uvX; }
	const int UVy() const { return uvY; }
};

class ItemDef {
private:
	static const map<ItemID, Item> defs;
public:
	static const Item& Get(ItemID id);
};