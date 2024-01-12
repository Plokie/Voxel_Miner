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
const map<BlockID, Block> BlockDef::def = {
	{BlockID::ERR,	Block("ERROR", B_CLIP, false, 0,
		0, 0
	) },
	{BlockID::AIR,		Block("Air", B_CLIP, false, 0,
		0 ,0
	)},

	{BlockID::GRASS,	Block("Grass", B_OPAQUE, true, 0,
		1, 0, //top
		2, 0, //middle
		3, 0, //bottom
		true, // has shell texture
		SHOVEL
	)},
	{BlockID::DIRT,		Block("Dirt", B_OPAQUE, true, 0,
		3, 0, false, SHOVEL
	)},
	{BlockID::STONE,	Block("Stone", B_OPAQUE, true, 0,
		5, 0, false, PICKAXE, 1,
		"STONE" //loot table name
	)},
	{BlockID::BLACKSTONE,Block("Blackstone", B_OPAQUE, true, 0,
		5, 1, false, PICKAXE, 1, "BLACKSTONE"
	)},
	{BlockID::SAND,		Block("Sand", B_OPAQUE, true, 0,
		4, 0
	)},
	{BlockID::CLAY,		Block("Clay", B_OPAQUE, true, 0,
		4, 3
	)},
	{BlockID::WATER,	Block("Water", B_TRANSPARENT, false, 0,
		4, 1, false, INDESTRUCTIBLE
	)},

	{BlockID::OAK_LOG,	Block("Oak Log", B_OPAQUE, true, 0,
		1, 1,
		0, 1,
		1, 1,
		false,
		AXE
	)},
	{BlockID::OAK_LEAVES,	Block("Oak Leaves", B_CLIP, false, 0,
		2, 1, false, BASICITEM, 0, "OAK_LEAVES"
	)},
	{BlockID::OAK_PLANKS,	Block("Oak Planks", B_OPAQUE, true, 0,
		3, 1,
		false,
		AXE
	)},

	{BlockID::LAMP,	Block("Lamp", B_CLIP, true, 15,
		3, 1
	)},

	{BlockID::SNOW_GRASS, Block("Snowy Grass", B_OPAQUE, true, 0,
		1, 9,
		0, 9,
		3, 0,
		false,
		SHOVEL
	)},

	{BlockID::TAIGA_GRASS, Block("Taiga Grass", B_OPAQUE, true, 0,
		1, 5,
		0, 5,
		3, 0,
		true,
		SHOVEL
	)},

	{BlockID::MAPLE_GRASS, Block("Maple Grass", B_OPAQUE, true, 0,
		1, 8,
		0, 8,
		3, 0,
		true,
		SHOVEL
	)},

	{BlockID::CHERRY_GRASS, Block("Cherry Grass", B_OPAQUE, true, 0,
		1, 6,
		0, 6,
		3, 0,
		true,
		SHOVEL
	)},

	{BlockID::BIRCH_LOG,	Block("Birch Log", B_OPAQUE, true, 0,
		1, 2,
		0, 2,
		1, 2,
		false,
		AXE
	)},
	{BlockID::BIRCH_LEAVES,	Block("Birch Leaves", B_CLIP, false, 0,
		2, 2, false, BASICITEM, 0, "BIRCH_LEAVES"
	)},
	{BlockID::BIRCH_PLANKS,	Block("Birch Planks", B_OPAQUE, true, 0,
		3, 2,
		false,
		AXE
	)},

	{BlockID::CHERRY_LOG,	Block("Cherry Log", B_OPAQUE, true, 0,
		1, 4,
		0, 4,
		1, 4,
		false,
		AXE
	)},
	{BlockID::CHERRY_LEAVES,	Block("Cherry Leaves", B_CLIP, false, 0,
		2, 4, false, BASICITEM, 0, "CHERRY_LEAVES"
	)},
	{BlockID::CHERRY_PLANKS,	Block("Cherry Planks", B_OPAQUE, true, 0,
		3, 4,
		false,
		AXE
	)},

	{BlockID::SPRUCE_LOG,	Block("Spruce Log", B_OPAQUE, true, 0,
		1, 3,
		0, 3,
		1, 3,
		false,
		AXE
	)},
	{BlockID::SPRUCE_LEAVES,	Block("Spruce Leaves", B_CLIP, false, 0,
		2, 3, false, BASICITEM, 0, "SPRUCE_LEAVES"
	)},
	{BlockID::SPRUCE_PLANKS,	Block("Spruce Planks", B_OPAQUE, true, 0,
		3, 3,
		false,
		AXE
	)},

	{ BlockID::COAL_ORE,	Block("Coal Ore", B_OPAQUE, true, 0,
		6, 0, false, PICKAXE, 1, "COAL"
	) },
	{ BlockID::COPPER_ORE,	Block("Copper Ore", B_OPAQUE, true, 0,
		7, 0, false, PICKAXE, 1, "COPPER"
	) },
	{ BlockID::GOLD_ORE,	Block("Gold Ore", B_OPAQUE, true, 0,
		8, 0, false, PICKAXE, 1, "GOLD"
	) },
	{ BlockID::AMETHYST_ORE,Block("Amethyst Ore", B_OPAQUE, true, 0,
		9, 0, false, PICKAXE, 2, "AMETHYST"
	) },
	{ BlockID::TITANIUM_ORE,Block("Titanium Ore", B_OPAQUE, true, 0,
		10, 0, false, PICKAXE, 3, "TITANIUM"
	) },
	{ BlockID::COBBLESTONE,Block("Cobblestone", B_OPAQUE, true, 0,
		11, 0, false, PICKAXE
	) },

	{ BlockID::BLACK_COAL_ORE,	Block("Blackstone Coal Ore", B_OPAQUE, true, 0,
		6, 1, false, PICKAXE, 1, "COAL"
	) },
	{ BlockID::BLACK_COPPER_ORE,	Block("Blackstone Copper Ore", B_OPAQUE, true, 0,
		7, 1, false, PICKAXE, 1, "COPPER"
	) },
	{ BlockID::BLACK_GOLD_ORE,	Block("Blackstone Gold Ore", B_OPAQUE, true, 0,
		8, 1, false, PICKAXE, 1, "GOLD"
	) },
	{ BlockID::BLACK_AMETHYST_ORE,Block("Blackstone Amethyst Ore", B_OPAQUE, true, 0,
		9, 1, false, PICKAXE, 2, "AMETHYST"
	) },
	{ BlockID::BLACK_TITANIUM_ORE,Block("Blackstone Titanium Ore", B_OPAQUE, true, 0,
		10, 1, false, PICKAXE, 3, "TITANIUM"
	) },
	{ BlockID::BLACK_COBBLESTONE,Block("Cobbled Blackstone", B_OPAQUE, true, 0,
		11, 1, false, PICKAXE
	) },

	{ BlockID::GRAVEL,Block("Gravel", B_OPAQUE, true, 0,
		4, 2, false, SHOVEL
	) },

	{ BlockID::LAVA, Block("Lava", B_CLIP, false, 13,
	4, 5, false, INDESTRUCTIBLE
	) },

	{ BlockID::FURNACE, Block("Furnace", B_OPAQUE, true, 0,
		3, 6,
		2, 5,
		3, 6,
		false, PICKAXE, 0
	) },
	{ BlockID::LIT_FURNACE, Block("Lit Furnace", B_OPAQUE, true, 13,
		3, 6,
		2, 6,
		3, 6,
		false, PICKAXE, 0, "LIT_FURNACE"
	) },
	{ BlockID::WORKBENCH, Block("Workbench", B_OPAQUE, true, 0,
		3, 7,
		2, 7,
		3, 1,
		false, AXE, 0
	) },
	{ BlockID::CHEST, Block("Chest", B_OPAQUE, true, 0,
		3, 8,
		2, 9,
		3, 8,
		false, AXE, 0
	) },

	{ BlockID::GLASS, Block("Glass", B_CLIP, true, 0,
		4, 4,
		false, BASICITEM, 0, ""
	) },

	{ BlockID::COAL_BLOCK, Block("Coal Block", B_OPAQUE, true, 0,
		6, 2,
		false, PICKAXE, 0, ""
	) },
	{ BlockID::COPPER_BLOCK, Block("Copper Block", B_OPAQUE, true, 0,
		7, 2,
		false, PICKAXE, 0, ""
	) },
	{ BlockID::GOLD_BLOCK, Block("Gold Block", B_OPAQUE, true, 0,
		8, 2,
		false, PICKAXE, 0, ""
	) },
	{ BlockID::AMETHYST_BLOCK, Block("Amethyst Block", B_OPAQUE, true, 0,
		9, 2,
		false, PICKAXE, 0, ""
	) },
	{ BlockID::TITANIUM_BLOCK, Block("Titanium Block", B_OPAQUE, true, 0,
		10, 2,
		false, PICKAXE, 0, ""
	) },

	{ BlockID::BRICK_BLOCK, Block("Brick Block", B_OPAQUE, true, 0,
		5, 3,
		false, PICKAXE, 0, ""
	) },
	{ BlockID::STONE_BRICKS, Block("Stone Bricks", B_OPAQUE, true, 0,
		12, 0,
		false, PICKAXE, 0, ""
	) },
	{ BlockID::BLACKSTONE_BRICKS, Block("Blackstone Bricks", B_OPAQUE, true, 0,
		12, 1,
		false, PICKAXE, 0, ""
	) },

	{ BlockID::BEDROCK, Block("Bedrock", B_OPAQUE, true, 0,
		4, 6,
		false, INDESTRUCTIBLE, 0, ""
	) },

	{ BlockID::CACTUS, Block("Cactus", B_OPAQUE, true, 0,
		1, 11,
		1, 10,
		2, 10,
		false, BASICITEM, 0, ""
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

const Block& BlockDef::GetDef(BlockID id)
{
	if(def.count(id)) {
		return BlockDef::def.at(id);
	}
	//assert(false);
	return BlockDef::def.at(BlockID::ERR);
}
