#pragma once

#include "FastNoiseLite.h"
#include "Blocks.h"

class WorldGen {
	FastNoiseLite noiseSampler_Height1;
	FastNoiseLite noiseSampler_Caves1;

	FastNoiseLite noiseSampler_Sky_Under;
	FastNoiseLite noiseSampler_Sky_Top;

	FastNoiseLite noiseSampler_Temperature;
	FastNoiseLite noiseSampler_Moisture;

	static WorldGen* _Instance;
public:
	WorldGen();

	static float SampleWorldHeight(const int& x, const int& z);
	static float SampleTemperature(const int& x, const int& z);
	static float SampleMoisture(const int& x, const int& z);
	static BlockID GetBlockAt(int x, int y, int z);
	static BlockID GetBlockGivenHeight(const int& x, const int& y, const int& z, const int& heightSample, const float& tempSample, const float& moistSample);
	static bool IsBlockCave(int x, int y, int z);
};
