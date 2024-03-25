#pragma once

#include <map>
#include <string>
#include "ItemTypes.h"
#include "ItemCategory.h"
#include "../Engine/MathUtil.h"
#include "BlockShape.h"

using namespace std;

#define ATLAS_SIZE 256
#define ATLAS_TILE_SIZE 16

#define MINEABLE_MASK (unsigned int)0b00000000000000000000000000011100

enum BLOCK_TAGS : unsigned int { 
	BT_DRAW_TRANSPARENT=1, BT_DRAW_CLIP=2,
	BT_MINEABLE_PICKAXE=4, BT_MINEABLE_AXE=8, BT_MINEABLE_SHOVEL=16,
	BT_NONSOLID=32,
	BT_SHELL=64,
	BT_SPAWNABLE_ANIMALS=128, BT_SPAWNABLE_ENEMIES=256,
	BT_INDESTRUCTIBLE=512, BT_MINEABLE_ALL=1024
};

enum DRAW_TYPE : unsigned char { B_OPAQUE, B_TRANSPARENT, B_CLIP };

enum BLOCK_FACE_TEX : unsigned char { FT_TOP, FT_SIDE, FT_BOTTOM };

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

	FURNACE, LIT_FURNACE,
	WORKBENCH,
	CHEST,
	
	GLASS,
	COAL_BLOCK, COPPER_BLOCK, GOLD_BLOCK, AMETHYST_BLOCK, TITANIUM_BLOCK,
	BRICK_BLOCK,
	STONE_BRICKS,
	BLACKSTONE_BRICKS,
	BEDROCK,
	CACTUS,

	BLOCK_COUNT
};

class Block {
private:
	string name;

	ITEM_CATEGORY category;
	//DRAW_TYPE draw_type; // Is the block opaque, transparent, or clip
	//bool isSolid = true;
	int lightValue = 0;

	int topUvIdX, topUvIdY;
	int bottUvIdX, bottUvIdY;
	int sideUvIdX, sideUvIdY;

	BlockShapeID blockShapeID;
	//int tier;

	//bool hasShell = false;
	//ItemType mineType = ItemType::BASICITEM;
	string lootTable = "";

	BLOCK_TAGS tags;

	
public:

	Block(string name, BlockShapeID blockShapeID, ITEM_CATEGORY category, int UvIdX, int UvIdY, uint32_t tags, int lightValue=0, const string& lootTable = "") :
		name(name), blockShapeID(blockShapeID), category(category), 
		topUvIdX(UvIdX), topUvIdY(UvIdY),
		bottUvIdX(UvIdX), bottUvIdY(UvIdY),
		sideUvIdX(UvIdX), sideUvIdY(UvIdY),
		tags((BLOCK_TAGS)tags), lightValue(lightValue),
		lootTable(lootTable){}

	Block(string name, BlockShapeID blockShapeID, ITEM_CATEGORY category,
		int TopUvIdX, int TopUvIdY,
		int SideUvIdX, int SideUvIdY,
		int BottUvIdX, int BottUvIdY,
		uint32_t tags, int lightValue = 0,
		const string& lootTable = "") :
		name(name), blockShapeID(blockShapeID), category(category),
		topUvIdX(TopUvIdX), topUvIdY(TopUvIdY),
		sideUvIdX(SideUvIdX), sideUvIdY(SideUvIdY),
		bottUvIdX(BottUvIdX), bottUvIdY(BottUvIdY),
		tags((BLOCK_TAGS)tags), lightValue(lightValue),
		lootTable(lootTable) {}

	//Block(string _Name,
	//	ITEM_CATEGORY category,
	//	DRAW_TYPE draw_type,
	//	bool isSolid,
	//	int _lightValue,
	//	int UvIdX, int UvIdY,
	//	//bool isSolid = true,
	//	bool hasShell = false,
	//	ItemType mineType = ItemType::BASICITEM,
	//	int tier=0,
	//	const string& lootTable = ""
	//) :
	//	name(_Name),
	//	category(category),
	//	draw_type(draw_type),
	//	isSolid(isSolid),
	//	lightValue(_lightValue),
	//	topUvIdX(UvIdX), topUvIdY(UvIdY),
	//	bottUvIdX(UvIdX), bottUvIdY(UvIdY),
	//	sideUvIdX(UvIdX), sideUvIdY(UvIdY),
	//	hasShell(hasShell),
	//	mineType(mineType),
	//	tier(tier),
	//	lootTable(lootTable)
	//{}

	//Block(string _Name,
	//	ITEM_CATEGORY category,
	//	DRAW_TYPE draw_type,
	//	bool isSolid,
	//	int _lightValue,
	//	int TopUvIdX, int TopUvIdY,
	//	int SideUvIdX, int SideUvIdY,
	//	int BottUvIdX, int BottUvIdY,
	//	//bool isSolid = true,
	//	bool hasShell = false,
	//	ItemType mineType = ItemType::BASICITEM,
	//	int tier = 0,
	//	const string& lootTable = ""
	//) :
	//	name(_Name),
	//	category(category),
	//	draw_type(draw_type),
	//	isSolid(isSolid),
	//	lightValue(_lightValue),
	//	topUvIdX(TopUvIdX), topUvIdY(TopUvIdY),
	//	sideUvIdX(SideUvIdX), sideUvIdY(SideUvIdY),
	//	bottUvIdX(BottUvIdX), bottUvIdY(BottUvIdY),
	//	hasShell(hasShell),
	//	mineType(mineType),
	//	tier(tier),
	//	lootTable(lootTable)
	//{}

	const string& GetName() const { return name; }
	//const DRAW_TYPE GetDrawType() const { return draw_type; }
	//const DRAW_TYPE GetDrawType() const { 
		
		//return draw_type; 
	//}
	const int LightValue() const;
	//const bool HasShell() const { return hasShell; }
	//const ItemType GetMineType() const { return mineType; }
	const string& GetLootTableName() const { return lootTable; }
	const int GetTier() const { return tags>>28; }
	//const bool IsSolid() const { return isSolid; }
	const bool HasTag(uint32_t tag) const { return tags & tag; }
	const ITEM_CATEGORY GetCategory() const { return category; }
	BlockShapeID GetBlockShapeID() const { return blockShapeID; }

	const int GetTopUVidx() const;
	const int GetTopUVidy() const;
	const int GetSideUVidx() const;
	const int GetSideUVidy() const;
	const int GetBottUVidx() const;
	const int GetBottUVidy() const;	

	const Vector2 GetBottomUV() const;
};

class BlockDef {
private:
	static const map<BlockID, Block> def;
public:
	static const Block& GetDef(BlockID id);
};