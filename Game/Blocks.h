#pragma once

#include <map>
#include <string>

using namespace std;

#define ATLAS_SIZE 256
#define ATLAS_TILE_SIZE 16

enum BlockID {
	ERR,
	AIR,

	GRASS,
	DIRT,
	STONE,
	BLACKSTONE,
	SAND,
	CLAY,

	WATER,

	OAK_LOG,
	OAK_LEAVES,
	OAK_PLANKS,


};

class Block {
private:
	string name;
	bool isSolid;
	int topUvIdX, topUvIdY;
	int bottUvIdX, bottUvIdY;
	int sideUvIdX, sideUvIdY;
public:

	Block(string _Name, 
		bool _IsSolid, 
		int UvIdX, int UvIdY
	): 
		name(_Name), 
		isSolid(_IsSolid), 
		topUvIdX(UvIdX), topUvIdY(UvIdY),
		bottUvIdX(UvIdX), bottUvIdY(UvIdY),
		sideUvIdX(UvIdX), sideUvIdY(UvIdY)
	{}

	Block(string _Name,
		bool _IsSolid,
		int TopUvIdX, int TopUvIdY,
		int SideUvIdX, int SideUvIdY,
		int BottUvIdX, int BottUvIdY
	) :
		name(_Name),
		isSolid(_IsSolid),
		topUvIdX(TopUvIdX), topUvIdY(TopUvIdY),
		sideUvIdX(SideUvIdX), sideUvIdY(SideUvIdY),
		bottUvIdX(BottUvIdX), bottUvIdY(BottUvIdY)
	{}

	const string GetName() const;
	const bool IsSolid() const;

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