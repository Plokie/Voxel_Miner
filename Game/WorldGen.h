#pragma once

#include "FastNoiseLite.h"
#include "Blocks.h"
#include "TerrainFeature.h"

class WorldGen {
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
