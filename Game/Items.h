#pragma once

#include <map>
#include <string>
#include "ItemTypes.h"
#include "BlockAction.h"
#include "../Engine/MathUtil.h"
#include "ItemCategory.h"


class BlockAction;
class PlayerController;
class Inventory;
class ChunkManager;

#define ITEM_ATLAS_SIZE 256
#define ITEM_ATLAS_TILE_SIZE 16
#define MAX_STACK 100

#define TIER_TAG 28

using namespace std;

enum ITEM_TAGS : unsigned int {
	IT_FOOD = 1, IT_UNUSED = 2,
	IT_PICKAXE = 4, IT_AXE = 8, IT_SHOVEL=16,
};

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
	APPLE, ROASTED_APPLE,

	ITEM_COUNT
};

class Item {
private:
	string name;
	ITEM_CATEGORY category;

	int uvX, uvY;
	unsigned short maxStack;
	ITEM_TAGS tags;

	static map<ItemID, BlockAction> itemActions;
public:
	Item(string name, ITEM_CATEGORY category, int uvX, int uvY, unsigned short maxStack = MAX_STACK, unsigned int tags = 0) :
		name(name), category(category), uvX(uvX), uvY(uvY), tags((ITEM_TAGS)tags), maxStack(maxStack) {}

	//Item(string name, ITEM_CATEGORY category, int uvX, int uvY, ItemType itemType, unsigned short maxStack=MAX_STACK) :
	//	name(name), category(category), uvX(uvX), uvY(uvY), itemType(itemType), maxStack(maxStack) {}

	const string& GetName() const { return name; }
	const int UVx() const { return uvX; }
	const int UVy() const { return uvY; }
	const unsigned short GetMaxStack() const { return maxStack; }
	//const ItemType GetItemType() const { return itemType; }
	const ITEM_TAGS GetTags() const { return tags; }
	const bool HasTag(unsigned int tag) { return tags & tag; }
	const int GetTier() const { return tags>>TIER_TAG; }
	const ITEM_CATEGORY GetCategory() const { return category; }

	static bool CallItemAction(ItemID itemID, PlayerController* playerController, PlayerData* inv, ChunkManager* chunkManager, Vector3Int targetBlockPos);
};

class ItemDef {
private:
	static const map<ItemID, Item> defs;
public:
	static const Item& Get(ItemID id);
};