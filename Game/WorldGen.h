#pragma once

#include <unordered_map>

#include "FastNoiseLite.h"
#include "Blocks.h"
#include "TerrainFeature.h"

enum BiomeID {
	GRASSLANDS,
	SNOW,
	DESERT,
	TAIGA,
	CHERRY,
	MAPLE
};
class Biome {
public:
	string name = "N/A";
	BlockID surface = ERR;
	BlockID earthTop = ERR;
	BlockID earthBottom = ERR;
	BlockID stone = ERR;

	BlockID shore = ERR;
	BlockID waterBed = ERR;
	BlockID sandTypeBottom = ERR;
	BlockID clayType = ERR;

	BlockID wood = ERR;
	BlockID leaves = ERR;

	Biome() = default;
	Biome(const string& name, BlockID surface, BlockID earthTop, BlockID earthBottom, BlockID stone, BlockID shore, BlockID waterBed, BlockID sandBottom, BlockID clayType, BlockID wood, BlockID leaves) :
		name(name), surface(surface), earthTop(earthTop), earthBottom(earthBottom), stone(stone), shore(shore), waterBed(waterBed), sandTypeBottom(sandBottom), clayType(clayType), wood(wood), leaves(leaves) {}

	static map<BiomeID, Biome> def;
	static vector<pair<BiomeID, AABB>> range;
	static const Biome& Get(float temperature, float moisture);

};

class WorldGen {


private:
	FastNoiseLite noiseSampler_HeightVal0;
	FastNoiseLite noiseSampler_HeightVal1;

	//FastNoiseLite noiseSampler_Height1;
	FastNoiseLite noiseSampler_Caves1;

	//FastNoiseLite noiseSampler_Sky_Under;
	//FastNoiseLite noiseSampler_Sky_Top;

	FastNoiseLite noiseSampler_Temperature;
	FastNoiseLite noiseSampler_Moisture;

	FastNoiseLite noiseSampler_Mountains;

	FastNoiseLite noiseSampler_treeValue;
	FastNoiseLite noiseSampler_treeDist;

	vector<TerrainFeature*> _pendingFeatures;

	static WorldGen* _Instance;
public:
	WorldGen();

	static float SampleWorldHeight(const int& x, const int& z);
	static float SampleTemperature(const int& x, const int& z);
	static float SampleMoisture(const int& x, const int& z);
	static BlockID GetBlockAt(const int& x, const int& y, const int& z);
	static BlockID GetBlockGivenHeight(const int& x, const int& y, const int& z, const int& heightSample, const Biome& biome, const float moisture);
	static bool IsBlockCave(const int& x, const int& y, const int& z);
};
