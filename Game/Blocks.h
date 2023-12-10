#pragma once

#include <map>
#include <string>
#include "ItemTypes.h"

using namespace std;

#define ATLAS_SIZE 256
#define ATLAS_TILE_SIZE 16

enum BlockID : unsigned short {
	ERR,
	AIR,

	GRASS, DIRT, STONE, BLACKSTONE, SAND, CLAY,

	WATER,

	OAK_LOG, OAK_LEAVES, OAK_PLANKS,

	LAMP,

	SNOW_GRASS, TAIGA_GRASS, MAPLE_GRASS, CHERRY_GRASS,

	BIRCH_LOG, BIRCH_LEAVES, BIRCH_PLANKS,
	
	SPRUCE_LOG, SPRUCE_LEAVES, SPRUCE_PLANKS,

	CHERRY_LOG, CHERRY_LEAVES, CHERRY_PLANKS,

	COAL_ORE, COPPER_ORE, GOLD_ORE, AMETHYST_ORE, TITANIUM_ORE, 
	COBBLESTONE,

	BLACK_COAL_ORE, BLACK_COPPER_ORE, BLACK_GOLD_ORE, BLACK_AMETHYST_ORE, BLACK_TITANIUM_ORE, 
	BLACK_COBBLESTONE,

	GRAVEL, LAVA,
};

class Block {
private:
	string name;

	bool isOpaque; // Is the block see-through
	int lightValue = 0;

	int topUvIdX, topUvIdY;
	int bottUvIdX, bottUvIdY;
	int sideUvIdX, sideUvIdY;

	bool hasShell = false;
	ItemType mineType = ItemType::BASICITEM;
	string lootTable = "";
public:

	Block(string _Name,
		bool _isOpaque,
		int _lightValue,
		int UvIdX, int UvIdY,
		bool hasShell = false,
		ItemType mineType = ItemType::BASICITEM,
		const string& lootTable = ""
	) :
		name(_Name),
		isOpaque(_isOpaque),
		lightValue(_lightValue),
		topUvIdX(UvIdX), topUvIdY(UvIdY),
		bottUvIdX(UvIdX), bottUvIdY(UvIdY),
		sideUvIdX(UvIdX), sideUvIdY(UvIdY),
		hasShell(hasShell),
		mineType(mineType),
		lootTable(lootTable)
	{}

	Block(string _Name,
		bool _isOpaque,
		int _lightValue,
		int TopUvIdX, int TopUvIdY,
		int SideUvIdX, int SideUvIdY,
		int BottUvIdX, int BottUvIdY,
		bool hasShell = false,
		ItemType mineType = ItemType::BASICITEM,
		const string& lootTable = ""
	) :
		name(_Name),
		isOpaque(_isOpaque),
		lightValue(_lightValue),
		topUvIdX(TopUvIdX), topUvIdY(TopUvIdY),
		sideUvIdX(SideUvIdX), sideUvIdY(SideUvIdY),
		bottUvIdX(BottUvIdX), bottUvIdY(BottUvIdY),
		hasShell(hasShell),
		mineType(mineType),
		lootTable(lootTable)
	{}

	const string& GetName() const { return name; }
	const bool IsOpaque() const;
	const int LightValue() const;
	const bool HasShell() const { return hasShell; }
	const ItemType GetMineType() const { return mineType; }
	const string& GetLootTableName() const { return lootTable; }

	const int GetTopUVidx() const;
	const int GetTopUVidy() const;
	const int GetSideUVidx() const;
	const int GetSideUVidy() const;
	const int GetBottUVidx() const;
	const int GetBottUVidy() const;
};

class BlockDef {
private:
	static const map<BlockID, Block> def;
public:
	static const Block& GetDef(BlockID id);
};