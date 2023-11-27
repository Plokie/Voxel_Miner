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
		true // has shell texture
	)},
	{BlockID::DIRT,		Block("Dirt", true, 0,
		3, 0
	)},
	{BlockID::STONE,	Block("Stone", true, 0,
		5, 0
	)},
	{BlockID::BLACKSTONE,Block("Blackstone", true, 0,
		5, 1
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
		1, 1
	)},
	{BlockID::OAK_LEAVES,	Block("Oak Leaves", false, 0,
		2, 1
	)},
	{BlockID::OAK_PLANKS,	Block("Oak Planks", true, 0,
		3, 1
	)},

	{BlockID::LAMP,	Block("Lamp", true, 15,
		3, 1
	)},

	{BlockID::SNOW_GRASS, Block("Snowy Grass", true, 0,
		1, 9,
		0, 9,
		3, 0
	)},

	{BlockID::TAIGA_GRASS, Block("Taiga Grass", true, 0,
		1, 5,
		0, 5,
		3, 0,
		true
	)},

	{BlockID::MAPLE_GRASS, Block("Maple Grass", true, 0,
		1, 8,
		0, 8,
		3, 0,
		true
	)},

	{BlockID::CHERRY_GRASS, Block("Cherry Grass", true, 0,
		1, 7,
		0, 7,
		3, 0,
		true
	)},

	{BlockID::BIRCH_LOG,	Block("Birch Log", true, 0,
		1, 2,
		0, 2,
		1, 2
	)},
	{BlockID::BIRCH_LEAVES,	Block("Birch Leaves", false, 0,
		2, 2
	)},
	{BlockID::BIRCH_PLANKS,	Block("Birch Planks", true, 0,
		3, 2
	)},

	{BlockID::CHERRY_LOG,	Block("Cherry Log", true, 0,
		1, 4,
		0, 4,
		1, 4
	)},
	{BlockID::CHERRY_LEAVES,	Block("Cherry Leaves", false, 0,
		2, 4
	)},
	{BlockID::CHERRY_PLANKS,	Block("Cherry Planks", true, 0,
		3, 4
	)},

	{BlockID::SPRUCE_LOG,	Block("Spruce Log", true, 0,
		1, 3,
		0, 3,
		1, 3
	)},
	{BlockID::SPRUCE_LEAVES,	Block("Spruce Leaves", false, 0,
		2, 3
	)},
	{BlockID::SPRUCE_PLANKS,	Block("Spruce Planks", true, 0,
		3, 3
	)},
};

const string Block::GetName() const {
	return this->name;
}

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
