#pragma once

#include "FastNoiseLite.h"
#include "Blocks.h"
#include "TerrainFeature.h"

class WorldGen {
public:
	enum BiomeID {
		GRASSLANDS
	};
	class Biome {
		const BlockID surface;
		const BlockID earthTop;
		const BlockID earthBottom;
		const BlockID stone;

		const BlockID wood;
		const BlockID leaves;

		Biome(BlockID surface, BlockID earthTop, BlockID earthBottom, BlockID stone, BlockID wood, BlockID leaves) : 
			surface(surface), earthTop(earthTop), earthBottom(earthBottom), stone(stone), wood(wood), leaves(leaves) {}
	
		static map<WorldGen::BiomeID, Biome> def;
		//static const Biome& Get(WorldGen::BiomeID id);
	};




private:
	FastNoiseLite noiseSampler_HeightVal0;
	FastNoiseLite noiseSampler_HeightVal1;

	FastNoiseLite noiseSampler_Height1;
	FastNoiseLite noiseSampler_Caves1;

	FastNoiseLite noiseSampler_Sky_Under;
	FastNoiseLite noiseSampler_Sky_Top;

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
	static BlockID GetBlockGivenHeight(const int& x, const int& y, const int& z, const int& heightSample, const float& tempSample, const float& moistSample);
	static bool IsBlockCave(const int& x, const int& y, const int& z);
};
