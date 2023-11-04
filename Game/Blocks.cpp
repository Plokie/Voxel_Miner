#include "Blocks.h"

#include <assert.h>

//todo: database?
// json parser perhaps
const map<BlockID, Block> BlockDef::def = {
	{BlockID::ERR,	Block("ERROR", false, 
		0, 0
	) },
	{BlockID::AIR,		Block("Air", false, 
		0 ,0
	)},

	{BlockID::GRASS,	Block("Grass", true, 
		1, 0,
		2, 0,
		3, 0
	)},
	{BlockID::DIRT,		Block("Dirt", true, 
		3, 0
	)},
	{BlockID::STONE,	Block("Stone", true, 
		5, 0
	)},
	{BlockID::BLACKSTONE,Block("Blackstone", true,
		5, 1
	)},
	{BlockID::SAND,		Block("Sand", true,
		4, 0
	)},
	{BlockID::CLAY,		Block("Clay", true,
		4, 3
	)},
	{BlockID::WATER,	Block("Water", false,
		4, 1
	)},
};

const string Block::GetName() const {
	return this->name;
}

const bool Block::IsSolid() const {
	return this->isSolid;
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
