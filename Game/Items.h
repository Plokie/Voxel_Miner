#pragma once

#include <map>
#include <string>
#include "ItemTypes.h"
#include "../Engine/MathUtil.h"

class BlockAction;
class PlayerController;
class Inventory;
class ChunkManager;

#define ITEM_ATLAS_SIZE 256
#define ITEM_ATLAS_TILE_SIZE 16
#define MAX_STACK 100

using namespace std;

enum ItemID : unsigned short {
	ITEMERR, COAL, RAW_COPPER, RAW_GOLD, AMETHYST, RAW_TITANIUM,
	COPPER_BAR, GOLD_BAR, TITANIUM_BAR,
	COPPER_PICKAXE, GOLD_PICKAXE, AMETHYST_PICKAXE, TITANIUM_PICKAXE,
	COPPER_AXE, GOLD_AXE, AMETHYST_AXE, TITANIUM_AXE,
	COPPER_SHOVEL, GOLD_SHOVEL, AMETHYST_SHOVEL, TITANIUM_SHOVEL,
	STICK,

	RAW_STEAK, COOKED_STEAK,
	RAW_CHICKEN, COOKED_CHICKEN,
	CARROT, ROASTED_CARROT,
	EGG, FRIED_EGG,
	APPLE, ROASTED_APPLE
};

class Item {
private:
	string name;

	int uvX, uvY;
	int maxStack;
	int tier;
	ItemType itemType = ItemType::BASICITEM;

	static map<ItemID, BlockAction> itemActions;
public:
	Item(string name, int uvX, int uvY, int maxStack=MAX_STACK, ItemType itemType = ItemType::BASICITEM, int tier = 0) : name(name), uvX(uvX), uvY(uvY), maxStack(maxStack), itemType(itemType), tier(tier) {}

	const string& GetName() const { return name; }
	const int UVx() const { return uvX; }
	const int UVy() const { return uvY; }
	const int GetMaxStack() const { return maxStack; }
	const ItemType GetItemType() const { return itemType; }
	const int GetTier() const { return tier; }

	static bool CallItemAction(ItemID itemID, PlayerController* playerController, Inventory* inv, ChunkManager* chunkManager, Vector3Int targetBlockPos);
};

class ItemDef {
private:
	static const map<ItemID, Item> defs;
public:
	static const Item& Get(ItemID id);
};