#pragma once

#include "FastNoiseLite.h"
#include "Blocks.h"

class WorldGen {
	FastNoiseLite noiseSampler;


	static WorldGen* _Instance;
public:
	WorldGen();

	static float SampleWorldHeight(int x, int z);
	static BlockID GetBlockAt(int x, int y, int z);
	static BlockID GetBlockGivenHeight(int x, int y, int z, int heightSample);
};
