#pragma once

#include <vector>
#include "../Engine/AABB.h"

#include "FastNoiseLite.h"
#include "Blocks.h"

class Biome;

class WorldGen {


private:
	FastNoiseLite noiseSampler_HeightVal0;
	FastNoiseLite noiseSampler_HeightVal1;

	//FastNoiseLite noiseSampler_Height1;
	FastNoiseLite noiseSampler_Caves1;
	FastNoiseLite noiseSampler_CavesTunnelsN0;
	FastNoiseLite noiseSampler_CavesTunnelsN1;

	FastNoiseLite noiseSampler_Rivers;

	//FastNoiseLite noiseSampler_Sky_Under;
	//FastNoiseLite noiseSampler_Sky_Top;

	FastNoiseLite noiseSampler_Temperature;
	FastNoiseLite noiseSampler_Moisture;
	FastNoiseLite noiseSampler_Oceans;

	FastNoiseLite noiseSampler_Mountains;

	FastNoiseLite noiseSampler_treeValue;
	FastNoiseLite noiseSampler_treeDist;


	FastNoiseLite noiseSampler_Cobbled;
	FastNoiseLite noiseSampler_CobbledPresence;

	//FastNoiseLite noiseSampler_C

	//vector<TerrainFeature*> _pendingFeatures;

	int seed = 1337;

	static WorldGen* _Instance;
public:
	WorldGen();

	static void SetSeed(int seed);
	static int GetSeed();
	void Init();

	static float SampleWorldHeight(const int& x, const int& z);
	static float SampleWorldSteepness(int x, int z);
	static float SampleTemperature(const int& x, const int& z);
	static float SampleOceanMap(int x, int z);
	static float SampleMoisture(const int& x, const int& z);
	static BlockID GetBlockAt(const int& x, const int& y, const int& z);
	static BlockID GetBlockGivenHeight(const int& x, const int& y, const int& z, const int& heightSample, const Biome& biome, const float moisture);
	static bool IsBlockCave(const int& x, const int& y, const int& z);
};
