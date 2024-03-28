#pragma once

#include "Blocks.h"

enum BiomeID {
	BIOME_GRASSLANDS,
	BIOME_FOREST,
	BIOME_FOREST_MOUNTAINS,
	BIOME_MOUNTAINS,
	BIOME_TUNDRA,
	BIOME_TUNDRA_MOUNTAINS,
	BIOME_SNOW,
	BIOME_SNOW_MOUNTAINS,
	BIOME_DESERT,
	BIOME_DESERT_MOUNTAINS,
	BIOME_TAIGA,
	BIOME_TAIGA_MOUNTAINS,
	BIOME_CHERRY,
	BIOME_CHERRY_MOUNTAINS,
	BIOME_MAPLE,
	BIOME_MAPLE_MOUNTAINS,
	BIOME_GRANITE_VALLEY,
	BIOME_JUNGLE,
	BIOME_BEACH,
	BIOME_OCEAN
};

enum BiomeTag : unsigned int {
	BIOMETAG = 1
};

enum NOISE_TYPE {
	NOISETYPE_DEFAULT, NOISETYPE_MOUNTAINS, NOISETYPE_OCEAN, NOISETYPE_BEACH, NOISETYPE_PLATEAU
};

struct BiomeFoliage {
	float foliagePresence = 0.2f;
	vector<pair<BlockID, int>> foliageWeights = {
		{AIR, 10},
		{TALL_GRASS, 1}
	};
};

struct BiomeSurface {
	BlockID top = GRASS;
	BlockID mid = DIRT;
	BlockID foundation = STONE;
	float maxTopSteepness = 3.f;
	
	BlockID sea_floor = GRAVEL;
};

struct BiomeTree {
	BlockID wood = OAK_LOG;
	BlockID leaves = OAK_LEAVES;
};


class Biome {
public:
	string name = "N/A";

	NOISE_TYPE noiseType = NOISETYPE_DEFAULT;
	
	BiomeSurface biomeSurface;
	vector<pair<BiomeTree, int>> treeWeights;
	BiomeFoliage biomeFoliage;

	Biome(){}
	//Biome(const string& name, BlockID test, NOISE_TYPE noiseType) : name(name), testBlock(test), noiseType(noiseType){}
	Biome(
		const string& name, 
		NOISE_TYPE noiseType,
		const BiomeSurface& biomeSurface, 
		const vector<pair<BiomeTree, int>> treeWeights, 
		const BiomeFoliage& biomeFoliage
	) :
		name(name), 
		noiseType(noiseType), 
		biomeSurface(biomeSurface), 
		treeWeights(treeWeights), 
		biomeFoliage(biomeFoliage)
	{}

	static map<BiomeID, Biome> def;
	static vector<pair<BiomeID, AABB>> range;
	static const Biome& Get(float temperature, float moisture, float ocean, float* confidence=nullptr);

};