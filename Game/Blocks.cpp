#include "Blocks.h"

#include <assert.h>

#include "PlayerController.h"
#include "Inventory.h"
#include "ChunkManager.h"

const map<BlockID, Block> BlockDef::def = {
	{BlockID::ERR,	Block("ERROR", BLOCKSHAPE_BLOCK, CAT_NONE, 0, 0, BT_NONSOLID) },
	{BlockID::AIR,		Block("Air", BLOCKSHAPE_BLOCK, CAT_NONE, 0 ,0, BT_DRAW_TRANSPARENT | BT_NONSOLID)},

	{BlockID::GRASS,	Block("Grass", BLOCKSHAPE_BLOCK, CAT_NATURE,
		1, 0, //top
		2, 0, //middle
		3, 0, //bottom
		BT_SHELL | BT_MINEABLE_SHOVEL
	)},
	{BlockID::DIRT,		Block("Dirt", BLOCKSHAPE_BLOCK, CAT_NATURE, 3, 0, BT_MINEABLE_SHOVEL)},
	{BlockID::STONE,	Block("Stone", BLOCKSHAPE_BLOCK, CAT_NATURE, 5, 0, BT_MINEABLE_PICKAXE, 0, "STONE")},
	{BlockID::BLACKSTONE,Block("Blackstone", BLOCKSHAPE_BLOCK, CAT_NATURE, 5, 1, BT_MINEABLE_PICKAXE, 0, "BLACKSTONE")},
	{BlockID::SAND,		Block("Sand", BLOCKSHAPE_BLOCK, CAT_NATURE, 4, 0, BT_MINEABLE_SHOVEL)},
	{BlockID::CLAY,		Block("Clay", BLOCKSHAPE_BLOCK, CAT_NATURE, 4, 3, BT_MINEABLE_SHOVEL)},
	{BlockID::WATER,	Block("Water", BLOCKSHAPE_BLOCK, CAT_NATURE, 4, 1, BT_DRAW_TRANSPARENT | BT_INDESTRUCTIBLE | BT_NONSOLID, 0, "", MESHFLAG::LIQUID)},

	{BlockID::OAK_LOG,	Block("Oak Log", BLOCKSHAPE_BLOCK, CAT_NATURE,
		1, 1,
		0, 1,
		1, 1,
		BT_MINEABLE_AXE
	)},
	{BlockID::OAK_LEAVES,	Block("Oak Leaves", BLOCKSHAPE_BLOCK, CAT_NATURE, 2, 1, BT_MINEABLE_ALL | BT_DRAW_CLIP | BT_NONSOLID, 0, "OAK_LEAVES", MESHFLAG::LEAVES)},
	{BlockID::OAK_PLANKS,	Block("Oak Planks", BLOCKSHAPE_BLOCK, CAT_BUILDING, 3, 1, BT_MINEABLE_AXE)},

	{BlockID::LAMP,	Block("Lamp", BLOCKSHAPE_BLOCK, CAT_BUILDING, 3, 1, 0, 15)},

	{BlockID::SNOW_GRASS, Block("Snowy Grass", BLOCKSHAPE_BLOCK, CAT_NATURE,
		1, 9,
		0, 9,
		3, 0,
		BT_MINEABLE_SHOVEL | BT_SHELL
	)},

	{BlockID::TAIGA_GRASS, Block("Taiga Grass", BLOCKSHAPE_BLOCK, CAT_NATURE,
		1, 5,
		0, 5,
		3, 0,
		BT_MINEABLE_SHOVEL | BT_SHELL
	)},

	{BlockID::MAPLE_GRASS, Block("Maple Grass", BLOCKSHAPE_BLOCK, CAT_NATURE,
		1, 8,
		0, 8,
		3, 0,
		BT_MINEABLE_SHOVEL | BT_SHELL
	)},

	{BlockID::CHERRY_GRASS, Block("Cherry Grass", BLOCKSHAPE_BLOCK, CAT_NATURE,
		1, 6,
		0, 6,
		3, 0,
		BT_MINEABLE_SHOVEL | BT_SHELL
	)},

	{BlockID::BIRCH_LOG,	Block("Birch Log", BLOCKSHAPE_BLOCK, CAT_NATURE,
		1, 2,
		0, 2,
		1, 2,
		BT_MINEABLE_AXE
	)},
	{BlockID::BIRCH_LEAVES,	Block("Birch Leaves", BLOCKSHAPE_BLOCK, CAT_NATURE, 2, 2, BT_MINEABLE_ALL | BT_DRAW_CLIP | BT_NONSOLID, 0, "BIRCH_LEAVES", MESHFLAG::LEAVES)},
	{BlockID::BIRCH_PLANKS,	Block("Birch Planks", BLOCKSHAPE_BLOCK, CAT_BUILDING, 3, 2,BT_MINEABLE_AXE)},

	{BlockID::CHERRY_LOG,	Block("Cherry Log", BLOCKSHAPE_BLOCK, CAT_NATURE,
		1, 4,
		0, 4,
		1, 4,
		BT_MINEABLE_AXE
	)},
	{BlockID::CHERRY_LEAVES,	Block("Cherry Leaves", BLOCKSHAPE_BLOCK, CAT_NATURE, 2, 4, BT_MINEABLE_ALL | BT_DRAW_CLIP | BT_NONSOLID, 0, "CHERRY_LEAVES", MESHFLAG::LEAVES
	)},
	{BlockID::CHERRY_PLANKS,	Block("Cherry Planks", BLOCKSHAPE_BLOCK, CAT_BUILDING, 3, 4, BT_MINEABLE_AXE )},

	{BlockID::SPRUCE_LOG,	Block("Spruce Log", BLOCKSHAPE_BLOCK, CAT_NATURE,
		1, 3,
		0, 3,
		1, 3,
		BT_MINEABLE_AXE
	)},
	{BlockID::SPRUCE_LEAVES,	Block("Spruce Leaves", BLOCKSHAPE_BLOCK, CAT_NATURE, 2, 3, BT_MINEABLE_ALL | BT_DRAW_CLIP | BT_NONSOLID, 0, "SPRUCE_LEAVES", MESHFLAG::LEAVES)},
	{BlockID::SPRUCE_PLANKS,	Block("Spruce Planks", BLOCKSHAPE_BLOCK, CAT_BUILDING,
		3, 3,
		BT_MINEABLE_AXE
	)},

	{ BlockID::COAL_ORE,	Block("Coal Ore", BLOCKSHAPE_BLOCK, CAT_NATURE, 6, 0, BT_MINEABLE_PICKAXE, 1, "COAL" ) },
	{ BlockID::COPPER_ORE,	Block("Copper Ore", BLOCKSHAPE_BLOCK, CAT_NATURE, 7, 0, BT_MINEABLE_PICKAXE, 1, "COPPER") },
	{ BlockID::GOLD_ORE,	Block("Gold Ore", BLOCKSHAPE_BLOCK, CAT_NATURE,
		8, 0, BT_MINEABLE_PICKAXE, 0, "GOLD"
	) },
	{ BlockID::AMETHYST_ORE,Block("Amethyst Ore", BLOCKSHAPE_BLOCK, CAT_NATURE,
		9, 0, BT_MINEABLE_PICKAXE, 2, "AMETHYST"
	) },
	{ BlockID::TITANIUM_ORE,Block("Titanium Ore", BLOCKSHAPE_BLOCK, CAT_NATURE,
		10, 0, BT_MINEABLE_PICKAXE, 0, "TITANIUM"
	) },
	{ BlockID::COBBLESTONE,Block("Cobblestone", BLOCKSHAPE_BLOCK, CAT_BUILDING,
		11, 0, BT_MINEABLE_PICKAXE
	) },

	{ BlockID::BLACK_COAL_ORE,	Block("Blackstone Coal Ore", BLOCKSHAPE_BLOCK, CAT_NATURE,
		6, 1, BT_MINEABLE_PICKAXE, 0, "COAL"
	) },
	{ BlockID::BLACK_COPPER_ORE,	Block("Blackstone Copper Ore", BLOCKSHAPE_BLOCK, CAT_NATURE,
		7, 1, BT_MINEABLE_PICKAXE, 0, "COPPER"
	) },
	{ BlockID::BLACK_GOLD_ORE,	Block("Blackstone Gold Ore", BLOCKSHAPE_BLOCK, CAT_NATURE,
		8, 1, BT_MINEABLE_PICKAXE, 0, "GOLD"
	) },
	{ BlockID::BLACK_AMETHYST_ORE,Block("Blackstone Amethyst Ore", BLOCKSHAPE_BLOCK, CAT_NATURE,
		9, 1, BT_MINEABLE_PICKAXE, 0, "AMETHYST"
	) },
	{ BlockID::BLACK_TITANIUM_ORE,Block("Blackstone Titanium Ore", BLOCKSHAPE_BLOCK, CAT_NATURE,
		10, 1, BT_MINEABLE_PICKAXE, 0, "TITANIUM"
	) },
	{ BlockID::BLACK_COBBLESTONE,Block("Cobbled Blackstone", BLOCKSHAPE_BLOCK, CAT_BUILDING, 11, 1, BT_MINEABLE_PICKAXE) },

	{ BlockID::GRAVEL,Block("Gravel", BLOCKSHAPE_BLOCK, CAT_NATURE, 4, 2, BT_MINEABLE_SHOVEL ) },

	{ BlockID::LAVA, Block("Lava", BLOCKSHAPE_BLOCK, CAT_NATURE, 4, 5, BT_DRAW_TRANSPARENT | BT_INDESTRUCTIBLE |BT_NONSOLID, 0, "", MESHFLAG::LIQUID)},

	{ BlockID::FURNACE, Block("Furnace", BLOCKSHAPE_BLOCK, CAT_UTILITY,
		3, 6,
		2, 5,
		3, 6,
		BT_MINEABLE_PICKAXE
	) },
	{ BlockID::LIT_FURNACE, Block("Lit Furnace", BLOCKSHAPE_BLOCK, CAT_UTILITY,
		3, 6,
		2, 6,
		3, 6,
		BT_MINEABLE_PICKAXE, 10, "LIT_FURNACE"
	) },
	{ BlockID::WORKBENCH, Block("Workbench", BLOCKSHAPE_BLOCK, CAT_UTILITY,
		3, 7,
		2, 7,
		3, 1,
		BT_MINEABLE_AXE
	) },
	{ BlockID::CHEST, Block("Chest", BLOCKSHAPE_BLOCK, CAT_UTILITY,
		3, 8,
		2, 9,
		3, 8,
		BT_MINEABLE_AXE
	) },

	{ BlockID::GLASS, Block("Glass", BLOCKSHAPE_BLOCK, CAT_BUILDING, 4, 4, BT_MINEABLE_ALL | BT_DRAW_CLIP ) },

	{ BlockID::COAL_BLOCK, Block("Coal Block", BLOCKSHAPE_BLOCK, CAT_BUILDING, 6, 2, BT_MINEABLE_PICKAXE ) },
	{ BlockID::COPPER_BLOCK, Block("Copper Block", BLOCKSHAPE_BLOCK, CAT_BUILDING, 7, 2, BT_MINEABLE_PICKAXE ) },
	{ BlockID::GOLD_BLOCK, Block("Gold Block", BLOCKSHAPE_BLOCK, CAT_BUILDING, 8, 2, BT_MINEABLE_PICKAXE ) },
	{ BlockID::AMETHYST_BLOCK, Block("Amethyst Block", BLOCKSHAPE_BLOCK, CAT_BUILDING, 9, 2, BT_MINEABLE_PICKAXE ) },
	{ BlockID::TITANIUM_BLOCK, Block("Titanium Block", BLOCKSHAPE_BLOCK, CAT_BUILDING, 10, 2, BT_MINEABLE_PICKAXE ) },

	{ BlockID::BRICK_BLOCK, Block("Brick Block", BLOCKSHAPE_BLOCK, CAT_BUILDING, 5, 3, BT_MINEABLE_PICKAXE ) },
	{ BlockID::STONE_BRICKS, Block("Stone Bricks", BLOCKSHAPE_BLOCK, CAT_BUILDING, 12, 0, BT_MINEABLE_PICKAXE ) },
	{ BlockID::BLACKSTONE_BRICKS, Block("Blackstone Bricks", BLOCKSHAPE_BLOCK, CAT_BUILDING, 12, 1, BT_MINEABLE_PICKAXE ) },

	{ BlockID::BEDROCK, Block("Bedrock", BLOCKSHAPE_BLOCK, CAT_NATURE, 4, 6, BT_INDESTRUCTIBLE ) },

	{ BlockID::CACTUS, Block("Cactus", BLOCKSHAPE_BLOCK, CAT_NATURE,
		1, 11,
		1, 10,
		2, 10,
		BT_MINEABLE_ALL
	) },

	{BlockID::ROSE, Block("Rose", BLOCKSHAPE_X, CAT_NATURE, 0, 15,				BT_MINEABLE_ALL | BT_NONSOLID | BT_GROUNDED | BT_FOLIAGE)},
	{BlockID::DANDELION, Block("Dandelion", BLOCKSHAPE_X, CAT_NATURE, 1, 15,	BT_MINEABLE_ALL | BT_NONSOLID | BT_GROUNDED | BT_FOLIAGE)},
	{BlockID::TALL_GRASS, Block("Tall Grass", BLOCKSHAPE_X, CAT_NATURE, 2, 15,	BT_MINEABLE_ALL | BT_NONSOLID | BT_GROUNDED | BT_FOLIAGE, 0, "", MESHFLAG::LEAVES)},

	{ BlockID::COBBLESTONE_SLAB, Block("Cobblestone Slab", BLOCKSHAPE_SLAB, CAT_BUILDING,
		11, 0, BT_MINEABLE_PICKAXE
	) },

	{ BlockID::SUGAR_CANE, Block("Sugar Cane", BLOCKSHAPE_X, CAT_NATURE, 3, 15,	BT_MINEABLE_ALL | BT_NONSOLID | BT_GROUNDED, 0, "", MESHFLAG::LEAVES) },
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
