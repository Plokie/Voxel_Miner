#include "Blocks.h"

#include <assert.h>

//todo: database?
// json parser perhaps
const map<BlockID, Block> BlockDef::def = {
	{BlockID::ERR,	Block("ERROR", false, 0,
		0, 0
	) },
	{BlockID::AIR,		Block("Air", false, 0,
		0 ,0
	)},

	{BlockID::GRASS,	Block("Grass", true, 0,
		1, 0,
		2, 0,
		3, 0,
		true, // has shell texture
		SHOVEL
	)},
	{BlockID::DIRT,		Block("Dirt", true, 0,
		3, 0, false, SHOVEL
	)},
	{BlockID::STONE,	Block("Stone", true, 0,
		5, 0, false, PICKAXE, 
		"STONE" //loot table name
	)},
	{BlockID::BLACKSTONE,Block("Blackstone", true, 0,
		5, 1, false, PICKAXE, "BLACKSTONE"
	)},
	{BlockID::SAND,		Block("Sand", true, 0,
		4, 0
	)},
	{BlockID::CLAY,		Block("Clay", true, 0,
		4, 3
	)},
	{BlockID::WATER,	Block("Water", false, 0,
		4, 1
	)},

	{BlockID::OAK_LOG,	Block("Oak Log", true, 0,
		1, 1,
		0, 1,
		1, 1,
		false,
		AXE
	)},
	{BlockID::OAK_LEAVES,	Block("Oak Leaves", false, 0,
		2, 1
	)},
	{BlockID::OAK_PLANKS,	Block("Oak Planks", true, 0,
		3, 1,
		false,
		AXE
	)},

	{BlockID::LAMP,	Block("Lamp", true, 15,
		3, 1
	)},

	{BlockID::SNOW_GRASS, Block("Snowy Grass", true, 0,
		1, 9,
		0, 9,
		3, 0,
		false,
		SHOVEL
	)},

	{BlockID::TAIGA_GRASS, Block("Taiga Grass", true, 0,
		1, 5,
		0, 5,
		3, 0,
		true,
		SHOVEL
	)},

	{BlockID::MAPLE_GRASS, Block("Maple Grass", true, 0,
		1, 8,
		0, 8,
		3, 0,
		true,
		SHOVEL
	)},

	{BlockID::CHERRY_GRASS, Block("Cherry Grass", true, 0,
		1, 7,
		0, 7,
		3, 0,
		true,
		SHOVEL
	)},

	{BlockID::BIRCH_LOG,	Block("Birch Log", true, 0,
		1, 2,
		0, 2,
		1, 2,
		false,
		AXE
	)},
	{BlockID::BIRCH_LEAVES,	Block("Birch Leaves", false, 0,
		2, 2
	)},
	{BlockID::BIRCH_PLANKS,	Block("Birch Planks", true, 0,
		3, 2,
		false,
		AXE
	)},

	{BlockID::CHERRY_LOG,	Block("Cherry Log", true, 0,
		1, 4,
		0, 4,
		1, 4,
		false,
		AXE
	)},
	{BlockID::CHERRY_LEAVES,	Block("Cherry Leaves", false, 0,
		2, 4
	)},
	{BlockID::CHERRY_PLANKS,	Block("Cherry Planks", true, 0,
		3, 4,
		false,
		AXE
	)},

	{BlockID::SPRUCE_LOG,	Block("Spruce Log", true, 0,
		1, 3,
		0, 3,
		1, 3,
		false,
		AXE
	)},
	{BlockID::SPRUCE_LEAVES,	Block("Spruce Leaves", false, 0,
		2, 3
	)},
	{BlockID::SPRUCE_PLANKS,	Block("Spruce Planks", true, 0,
		3, 3,
		false,
		AXE
	)},

	{ BlockID::COAL_ORE,	Block("Coal Ore", true, 0,
		6, 0, false, PICKAXE, "COAL"
	) },
	{ BlockID::COPPER_ORE,	Block("Copper Ore", true, 0,
		7, 0, false, PICKAXE, "COPPER"
	) },
	{ BlockID::GOLD_ORE,	Block("Gold Ore", true, 0,
		8, 0, false, PICKAXE, "GOLD"
	) },
	{ BlockID::AMETHYST_ORE,Block("Amethyst Ore", true, 0,
		9, 0, false, PICKAXE, "AMETHYST"
	) },
	{ BlockID::TITANIUM_ORE,Block("Titanium Ore", true, 0,
		10, 0, false, PICKAXE, "TITANIUM"
	) },
	{ BlockID::COBBLESTONE,Block("Cobblestone", true, 0,
		11, 0, false, PICKAXE
	) },

	{ BlockID::BLACK_COAL_ORE,	Block("Blackstone Coal Ore", true, 0,
		6, 1, false, PICKAXE, "COAL"
	) },
	{ BlockID::BLACK_COPPER_ORE,	Block("Blackstone Copper Ore", true, 0,
		7, 1, false, PICKAXE, "COPPER"
	) },
	{ BlockID::BLACK_GOLD_ORE,	Block("Blackstone Gold Ore", true, 0,
		8, 1, false, PICKAXE, "GOLD"
	) },
	{ BlockID::BLACK_AMETHYST_ORE,Block("Blackstone Amethyst Ore", true, 0,
		9, 1, false, PICKAXE, "AMETHYST"
	) },
	{ BlockID::BLACK_TITANIUM_ORE,Block("Blackstone Titanium Ore", true, 0,
		10, 1, false, PICKAXE, "TITANIUM"
	) },
	{ BlockID::BLACK_COBBLESTONE,Block("Cobbled Blackstone", true, 0,
		11, 1, false, PICKAXE
	) },

	{ BlockID::GRAVEL,Block("Gravel", true, 0,
		4, 2, false, SHOVEL
	) },

	{ BlockID::LAVA, Block("Lava", false, 13,
		4, 5
	) },
};
const bool Block::IsOpaque() const {
	return this->isOpaque;
}

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
