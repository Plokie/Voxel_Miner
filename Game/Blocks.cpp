#include "Blocks.h"

#include <assert.h>

#include "PlayerController.h"
#include "Inventory.h"
#include "ChunkManager.h"






//bool LitFurnaceFunc(BlockActionContext ctx) {
//	InventoryItem* heldItem = nullptr;
//	if(ctx.inventory->GetHeldItem(&heldItem)) {
//		if(heldItem->type == InventoryItem::ITEM) {
//			auto it = cookables.find((ItemID)heldItem->ID);
//			if(it != cookables.end()) {
//
//				//if(heldItem->Is(RAW_STEAK)) {
//				ctx.inventory->SubHeldItem();
//				ctx.inventory->AddItem(it->second);
//				return true;
//				//}
//			}
//
//		}
//	}
//	return false;
//}

//todo: database?
// json parser perhaps
//const map<BlockID, Block> BlockDef::def = {
//	{BlockID::ERR,	Block("ERROR", CAT_NONE, B_CLIP, false, 0,
//		0, 0
//	) },
//	{BlockID::AIR,		Block("Air", CAT_NONE, B_CLIP, false, 0,
//		0 ,0
//	)},
//
//	{BlockID::GRASS,	Block("Grass", CAT_NATURE, B_OPAQUE, true, 0,
//		1, 0, //top
//		2, 0, //middle
//		3, 0, //bottom
//		true, // has shell texture
//		SHOVEL
//	)},
//	{BlockID::DIRT,		Block("Dirt", CAT_NATURE, B_OPAQUE, true, 0,
//		3, 0, false, SHOVEL
//	)},
//	{BlockID::STONE,	Block("Stone", CAT_NATURE, B_OPAQUE, true, 0,
//		5, 0, false, PICKAXE, 1,
//		"STONE" //loot table name
//	)},
//	{BlockID::BLACKSTONE,Block("Blackstone", CAT_NATURE, B_OPAQUE, true, 0,
//		5, 1, false, PICKAXE, 1, "BLACKSTONE"
//	)},
//	{BlockID::SAND,		Block("Sand", CAT_NATURE, B_OPAQUE, true, 0,
//		4, 0
//	)},
//	{BlockID::CLAY,		Block("Clay", CAT_NATURE, B_OPAQUE, true, 0,
//		4, 3
//	)},
//	{BlockID::WATER,	Block("Water", CAT_NATURE, B_TRANSPARENT, false, 0,
//		4, 1, false, INDESTRUCTIBLE
//	)},
//
//	{BlockID::OAK_LOG,	Block("Oak Log", CAT_NATURE, B_OPAQUE, true, 0,
//		1, 1,
//		0, 1,
//		1, 1,
//		false,
//		AXE
//	)},
//	{BlockID::OAK_LEAVES,	Block("Oak Leaves", CAT_NATURE, B_CLIP, false, 0,
//		2, 1, false, BASICITEM, 0, "OAK_LEAVES"
//	)},
//	{BlockID::OAK_PLANKS,	Block("Oak Planks", CAT_BUILDING, B_OPAQUE, true, 0,
//		3, 1,
//		false,
//		AXE
//	)},
//
//	{BlockID::LAMP,	Block("Lamp", CAT_BUILDING, B_CLIP, true, 15,
//		3, 1
//	)},
//
//	{BlockID::SNOW_GRASS, Block("Snowy Grass", CAT_NATURE, B_OPAQUE, true, 0,
//		1, 9,
//		0, 9,
//		3, 0,
//		false,
//		SHOVEL
//	)},
//
//	{BlockID::TAIGA_GRASS, Block("Taiga Grass", CAT_NATURE, B_OPAQUE, true, 0,
//		1, 5,
//		0, 5,
//		3, 0,
//		true,
//		SHOVEL
//	)},
//
//	{BlockID::MAPLE_GRASS, Block("Maple Grass", CAT_NATURE, B_OPAQUE, true, 0,
//		1, 8,
//		0, 8,
//		3, 0,
//		true,
//		SHOVEL
//	)},
//
//	{BlockID::CHERRY_GRASS, Block("Cherry Grass", CAT_NATURE, B_OPAQUE, true, 0,
//		1, 6,
//		0, 6,
//		3, 0,
//		true,
//		SHOVEL
//	)},
//
//	{BlockID::BIRCH_LOG,	Block("Birch Log", CAT_NATURE, B_OPAQUE, true, 0,
//		1, 2,
//		0, 2,
//		1, 2,
//		false,
//		AXE
//	)},
//	{BlockID::BIRCH_LEAVES,	Block("Birch Leaves", CAT_NATURE, B_CLIP, false, 0,
//		2, 2, false, BASICITEM, 0, "BIRCH_LEAVES"
//	)},
//	{BlockID::BIRCH_PLANKS,	Block("Birch Planks", CAT_BUILDING, B_OPAQUE, true, 0,
//		3, 2,
//		false,
//		AXE
//	)},
//
//	{BlockID::CHERRY_LOG,	Block("Cherry Log", CAT_NATURE, B_OPAQUE, true, 0,
//		1, 4,
//		0, 4,
//		1, 4,
//		false,
//		AXE
//	)},
//	{BlockID::CHERRY_LEAVES,	Block("Cherry Leaves", CAT_NATURE, B_CLIP, false, 0,
//		2, 4, false, BASICITEM, 0, "CHERRY_LEAVES"
//	)},
//	{BlockID::CHERRY_PLANKS,	Block("Cherry Planks", CAT_BUILDING, B_OPAQUE, true, 0,
//		3, 4,
//		false,
//		AXE
//	)},
//
//	{BlockID::SPRUCE_LOG,	Block("Spruce Log", CAT_NATURE, B_OPAQUE, true, 0,
//		1, 3,
//		0, 3,
//		1, 3,
//		false,
//		AXE
//	)},
//	{BlockID::SPRUCE_LEAVES,	Block("Spruce Leaves", CAT_NATURE, B_CLIP, false, 0,
//		2, 3, false, BASICITEM, 0, "SPRUCE_LEAVES"
//	)},
//	{BlockID::SPRUCE_PLANKS,	Block("Spruce Planks", CAT_BUILDING, B_OPAQUE, true, 0,
//		3, 3,
//		false,
//		AXE
//	)},
//
//	{ BlockID::COAL_ORE,	Block("Coal Ore", CAT_NATURE, B_OPAQUE, true, 0,
//		6, 0, false, PICKAXE, 1, "COAL"
//	) },
//	{ BlockID::COPPER_ORE,	Block("Copper Ore", CAT_NATURE, B_OPAQUE, true, 0,
//		7, 0, false, PICKAXE, 1, "COPPER"
//	) },
//	{ BlockID::GOLD_ORE,	Block("Gold Ore", CAT_NATURE, B_OPAQUE, true, 0,
//		8, 0, false, PICKAXE, 1, "GOLD"
//	) },
//	{ BlockID::AMETHYST_ORE,Block("Amethyst Ore", CAT_NATURE, B_OPAQUE, true, 0,
//		9, 0, false, PICKAXE, 2, "AMETHYST"
//	) },
//	{ BlockID::TITANIUM_ORE,Block("Titanium Ore", CAT_NATURE, B_OPAQUE, true, 0,
//		10, 0, false, PICKAXE, 3, "TITANIUM"
//	) },
//	{ BlockID::COBBLESTONE,Block("Cobblestone", CAT_BUILDING, B_OPAQUE, true, 0,
//		11, 0, false, PICKAXE
//	) },
//
//	{ BlockID::BLACK_COAL_ORE,	Block("Blackstone Coal Ore", CAT_NATURE, B_OPAQUE, true, 0,
//		6, 1, false, PICKAXE, 1, "COAL"
//	) },
//	{ BlockID::BLACK_COPPER_ORE,	Block("Blackstone Copper Ore", CAT_NATURE, B_OPAQUE, true, 0,
//		7, 1, false, PICKAXE, 1, "COPPER"
//	) },
//	{ BlockID::BLACK_GOLD_ORE,	Block("Blackstone Gold Ore", CAT_NATURE, B_OPAQUE, true, 0,
//		8, 1, false, PICKAXE, 1, "GOLD"
//	) },
//	{ BlockID::BLACK_AMETHYST_ORE,Block("Blackstone Amethyst Ore", CAT_NATURE, B_OPAQUE, true, 0,
//		9, 1, false, PICKAXE, 2, "AMETHYST"
//	) },
//	{ BlockID::BLACK_TITANIUM_ORE,Block("Blackstone Titanium Ore", CAT_NATURE, B_OPAQUE, true, 0,
//		10, 1, false, PICKAXE, 3, "TITANIUM"
//	) },
//	{ BlockID::BLACK_COBBLESTONE,Block("Cobbled Blackstone", CAT_BUILDING,  B_OPAQUE, true, 0,
//		11, 1, false, PICKAXE
//	) },
//
//	{ BlockID::GRAVEL,Block("Gravel", CAT_NATURE, B_OPAQUE, true, 0,
//		4, 2, false, SHOVEL
//	) },
//
//	{ BlockID::LAVA, Block("Lava", CAT_NATURE, B_CLIP, false, 13,
//	4, 5, false, INDESTRUCTIBLE
//	) },
//
//	{ BlockID::FURNACE, Block("Furnace", CAT_UTILITY, B_OPAQUE, true, 0,
//		3, 6,
//		2, 5,
//		3, 6,
//		false, PICKAXE, 0
//	) },
//	{ BlockID::LIT_FURNACE, Block("Lit Furnace", CAT_UTILITY, B_OPAQUE, true, 13,
//		3, 6,
//		2, 6,
//		3, 6,
//		false, PICKAXE, 0, "LIT_FURNACE"
//	) },
//	{ BlockID::WORKBENCH, Block("Workbench", CAT_UTILITY, B_OPAQUE, true, 0,
//		3, 7,
//		2, 7,
//		3, 1,
//		false, AXE, 0
//	) },
//	{ BlockID::CHEST, Block("Chest", CAT_UTILITY, B_OPAQUE, true, 0,
//		3, 8,
//		2, 9,
//		3, 8,
//		false, AXE, 0
//	) },
//
//	{ BlockID::GLASS, Block("Glass", CAT_BUILDING, B_CLIP, true, 0,
//		4, 4,
//		false, BASICITEM, 0, ""
//	) },
//
//	{ BlockID::COAL_BLOCK, Block("Coal Block", CAT_BUILDING, B_OPAQUE, true, 0,
//		6, 2,
//		false, PICKAXE, 0, ""
//	) },
//	{ BlockID::COPPER_BLOCK, Block("Copper Block", CAT_BUILDING, B_OPAQUE, true, 0,
//		7, 2,
//		false, PICKAXE, 0, ""
//	) },
//	{ BlockID::GOLD_BLOCK, Block("Gold Block", CAT_BUILDING, B_OPAQUE, true, 0,
//		8, 2,
//		false, PICKAXE, 0, ""
//	) },
//	{ BlockID::AMETHYST_BLOCK, Block("Amethyst Block", CAT_BUILDING, B_OPAQUE, true, 0,
//		9, 2,
//		false, PICKAXE, 0, ""
//	) },
//	{ BlockID::TITANIUM_BLOCK, Block("Titanium Block", CAT_BUILDING, B_OPAQUE, true, 0,
//		10, 2,
//		false, PICKAXE, 0, ""
//	) },
//
//	{ BlockID::BRICK_BLOCK, Block("Brick Block", CAT_BUILDING, B_OPAQUE, true, 0,
//		5, 3,
//		false, PICKAXE, 0, ""
//	) },
//	{ BlockID::STONE_BRICKS, Block("Stone Bricks", CAT_BUILDING, B_OPAQUE, true, 0,
//		12, 0,
//		false, PICKAXE, 0, ""
//	) },
//	{ BlockID::BLACKSTONE_BRICKS, Block("Blackstone Bricks", CAT_BUILDING, B_OPAQUE, true, 0,
//		12, 1,
//		false, PICKAXE, 0, ""
//	) },
//
//	{ BlockID::BEDROCK, Block("Bedrock", CAT_NATURE, B_OPAQUE, true, 0,
//		4, 6,
//		false, INDESTRUCTIBLE, 0, ""
//	) },
//
//	{ BlockID::CACTUS, Block("Cactus", CAT_NATURE, B_OPAQUE, true, 0,
//		1, 11,
//		1, 10,
//		2, 10,
//		false, BASICITEM, 0, ""
//	) },
//};

const map<BlockID, Block> BlockDef::def = {
	{BlockID::ERR,	Block("ERROR", CAT_NONE, 0, 0, BT_NONSOLID) },
	{BlockID::AIR,		Block("Air", CAT_NONE, 0 ,0, BT_DRAW_TRANSPARENT | BT_NONSOLID)},

	{BlockID::GRASS,	Block("Grass", CAT_NATURE,
		1, 0, //top
		2, 0, //middle
		3, 0, //bottom
		BT_SHELL | BT_MINEABLE_SHOVEL
	)},
	{BlockID::DIRT,		Block("Dirt", CAT_NATURE, 3, 0, BT_MINEABLE_SHOVEL)},
	{BlockID::STONE,	Block("Stone", CAT_NATURE, 5, 0, BT_MINEABLE_PICKAXE, 0, "STONE")},
	{BlockID::BLACKSTONE,Block("Blackstone", CAT_NATURE, 5, 1, BT_MINEABLE_PICKAXE, 0, "BLACKSTONE")},
	{BlockID::SAND,		Block("Sand", CAT_NATURE, 4, 0, BT_MINEABLE_SHOVEL)},
	{BlockID::CLAY,		Block("Clay", CAT_NATURE, 4, 3, BT_MINEABLE_SHOVEL)},
	{BlockID::WATER,	Block("Water", CAT_NATURE, 4, 1, BT_DRAW_TRANSPARENT | BT_INDESTRUCTIBLE | BT_NONSOLID)},

	{BlockID::OAK_LOG,	Block("Oak Log", CAT_NATURE,
		1, 1,
		0, 1,
		1, 1,
		BT_MINEABLE_AXE
	)},
	{BlockID::OAK_LEAVES,	Block("Oak Leaves", CAT_NATURE, 
		2, 1, BT_MINEABLE_ALL | BT_DRAW_CLIP, 0, "OAK_LEAVES")},
	{BlockID::OAK_PLANKS,	Block("Oak Planks", CAT_BUILDING, 3, 1, BT_MINEABLE_AXE)},

	{BlockID::LAMP,	Block("Lamp", CAT_BUILDING, 3, 1, 0, 15)},

	{BlockID::SNOW_GRASS, Block("Snowy Grass", CAT_NATURE,
		1, 9,
		0, 9,
		3, 0,
		BT_MINEABLE_SHOVEL
	)},

	{BlockID::TAIGA_GRASS, Block("Taiga Grass", CAT_NATURE,
		1, 5,
		0, 5,
		3, 0,
		BT_MINEABLE_SHOVEL | BT_SHELL
	)},

	{BlockID::MAPLE_GRASS, Block("Maple Grass", CAT_NATURE,
		1, 8,
		0, 8,
		3, 0,
		BT_MINEABLE_SHOVEL | BT_SHELL
	)},

	{BlockID::CHERRY_GRASS, Block("Cherry Grass", CAT_NATURE,
		1, 6,
		0, 6,
		3, 0,
		BT_MINEABLE_SHOVEL | BT_SHELL
	)},

	{BlockID::BIRCH_LOG,	Block("Birch Log", CAT_NATURE,
		1, 2,
		0, 2,
		1, 2,
		BT_MINEABLE_AXE
	)},
	{BlockID::BIRCH_LEAVES,	Block("Birch Leaves", CAT_NATURE, 
		2, 2, BT_MINEABLE_ALL | BT_DRAW_CLIP | BT_NONSOLID, 0, "BIRCH_LEAVES")},
	{BlockID::BIRCH_PLANKS,	Block("Birch Planks", CAT_BUILDING, 3, 2,BT_MINEABLE_AXE)},

	{BlockID::CHERRY_LOG,	Block("Cherry Log", CAT_NATURE,
		1, 4,
		0, 4,
		1, 4,
		BT_MINEABLE_AXE
	)},
	{BlockID::CHERRY_LEAVES,	Block("Cherry Leaves", CAT_NATURE,
		2, 4, BT_MINEABLE_ALL | BT_DRAW_CLIP | BT_NONSOLID, 0, "CHERRY_LEAVES"
	)},
	{BlockID::CHERRY_PLANKS,	Block("Cherry Planks", CAT_BUILDING, 3, 4, BT_MINEABLE_AXE )},

	{BlockID::SPRUCE_LOG,	Block("Spruce Log", CAT_NATURE,
		1, 3,
		0, 3,
		1, 3,
		BT_MINEABLE_AXE
	)},
	{BlockID::SPRUCE_LEAVES,	Block("Spruce Leaves", CAT_NATURE, 
		2, 3, BT_MINEABLE_ALL | BT_DRAW_CLIP | BT_NONSOLID, 0, "SPRUCE_LEAVES" )},
	{BlockID::SPRUCE_PLANKS,	Block("Spruce Planks", CAT_BUILDING,
		3, 3,
		BT_MINEABLE_AXE
	)},

	{ BlockID::COAL_ORE,	Block("Coal Ore", CAT_NATURE, 6, 0, BT_MINEABLE_PICKAXE, 1, "COAL" ) },
	{ BlockID::COPPER_ORE,	Block("Copper Ore", CAT_NATURE, 7, 0, BT_MINEABLE_PICKAXE, 1, "COPPER") },
	{ BlockID::GOLD_ORE,	Block("Gold Ore", CAT_NATURE,
		8, 0, BT_MINEABLE_PICKAXE, 0, "GOLD"
	) },
	{ BlockID::AMETHYST_ORE,Block("Amethyst Ore", CAT_NATURE,
		9, 0, BT_MINEABLE_PICKAXE, 2, "AMETHYST"
	) },
	{ BlockID::TITANIUM_ORE,Block("Titanium Ore", CAT_NATURE,
		10, 0, BT_MINEABLE_PICKAXE, 0, "TITANIUM"
	) },
	{ BlockID::COBBLESTONE,Block("Cobblestone", CAT_BUILDING,
		11, 0, BT_MINEABLE_PICKAXE
	) },

	{ BlockID::BLACK_COAL_ORE,	Block("Blackstone Coal Ore", CAT_NATURE,
		6, 1, BT_MINEABLE_PICKAXE, 0, "COAL"
	) },
	{ BlockID::BLACK_COPPER_ORE,	Block("Blackstone Copper Ore", CAT_NATURE,
		7, 1, BT_MINEABLE_PICKAXE, 0, "COPPER"
	) },
	{ BlockID::BLACK_GOLD_ORE,	Block("Blackstone Gold Ore", CAT_NATURE,
		8, 1, BT_MINEABLE_PICKAXE, 0, "GOLD"
	) },
	{ BlockID::BLACK_AMETHYST_ORE,Block("Blackstone Amethyst Ore", CAT_NATURE,
		9, 1, BT_MINEABLE_PICKAXE, 0, "AMETHYST"
	) },
	{ BlockID::BLACK_TITANIUM_ORE,Block("Blackstone Titanium Ore", CAT_NATURE,
		10, 1, BT_MINEABLE_PICKAXE, 0, "TITANIUM"
	) },
	{ BlockID::BLACK_COBBLESTONE,Block("Cobbled Blackstone", CAT_BUILDING, 11, 1, BT_MINEABLE_PICKAXE) },

	{ BlockID::GRAVEL,Block("Gravel", CAT_NATURE, 4, 2, BT_MINEABLE_SHOVEL ) },

	{ BlockID::LAVA, Block("Lava", CAT_NATURE, 4, 5, BT_DRAW_TRANSPARENT | BT_INDESTRUCTIBLE |BT_NONSOLID ) },

	{ BlockID::FURNACE, Block("Furnace", CAT_UTILITY,
		3, 6,
		2, 5,
		3, 6,
		BT_MINEABLE_PICKAXE
	) },
	{ BlockID::LIT_FURNACE, Block("Lit Furnace", CAT_UTILITY,
		3, 6,
		2, 6,
		3, 6,
		BT_MINEABLE_PICKAXE, 10, "LIT_FURNACE"
	) },
	{ BlockID::WORKBENCH, Block("Workbench", CAT_UTILITY,
		3, 7,
		2, 7,
		3, 1,
		BT_MINEABLE_AXE
	) },
	{ BlockID::CHEST, Block("Chest", CAT_UTILITY,
		3, 8,
		2, 9,
		3, 8,
		BT_MINEABLE_AXE
	) },

	{ BlockID::GLASS, Block("Glass", CAT_BUILDING, 4, 4, BT_MINEABLE_ALL | BT_DRAW_CLIP ) },

	{ BlockID::COAL_BLOCK, Block("Coal Block", CAT_BUILDING, 6, 2, BT_MINEABLE_PICKAXE ) },
	{ BlockID::COPPER_BLOCK, Block("Copper Block", CAT_BUILDING, 7, 2, BT_MINEABLE_PICKAXE ) },
	{ BlockID::GOLD_BLOCK, Block("Gold Block", CAT_BUILDING, 8, 2, BT_MINEABLE_PICKAXE ) },
	{ BlockID::AMETHYST_BLOCK, Block("Amethyst Block", CAT_BUILDING, 9, 2, BT_MINEABLE_PICKAXE ) },
	{ BlockID::TITANIUM_BLOCK, Block("Titanium Block", CAT_BUILDING, 10, 2, BT_MINEABLE_PICKAXE ) },

	{ BlockID::BRICK_BLOCK, Block("Brick Block", CAT_BUILDING, 5, 3, BT_MINEABLE_PICKAXE ) },
	{ BlockID::STONE_BRICKS, Block("Stone Bricks", CAT_BUILDING, 12, 0, BT_MINEABLE_PICKAXE ) },
	{ BlockID::BLACKSTONE_BRICKS, Block("Blackstone Bricks", CAT_BUILDING, 12, 1, BT_MINEABLE_PICKAXE ) },

	{ BlockID::BEDROCK, Block("Bedrock", CAT_NATURE, 4, 6, BT_INDESTRUCTIBLE ) },

	{ BlockID::CACTUS, Block("Cactus", CAT_NATURE,
		1, 11,
		1, 10,
		2, 10,
		BT_MINEABLE_ALL
	) },
};

const int Block::LightValue() const {
	return this->lightValue;
}

const int Block::GetTopUVidx() const {
	return this->topUvIdX;
}
const int Block::GetTopUVidy() const {
	return this->topUvIdY;
}
const int Block::GetSideUVidx() const {
	return this->sideUvIdX;
}
const int Block::GetSideUVidy() const {
	return this->sideUvIdY;
}
const int Block::GetBottUVidx() const {
	return this->bottUvIdX;
}
const int Block::GetBottUVidy() const {
	return this->bottUvIdY;
}

const Vector2 Block::GetBottomUV() const
{
	Vector2 uv = { (float)this->bottUvIdX, (float)this->bottUvIdY };
	uv /= ATLAS_TILE_SIZE;
	return uv;
}

const Block& BlockDef::GetDef(BlockID id)
{
	if(def.count(id)) {
		return BlockDef::def.at(id);
	}
	//assert(false);
	return BlockDef::def.at(BlockID::ERR);
}
