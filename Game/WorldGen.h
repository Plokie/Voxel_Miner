#pragma once

#include "FastNoiseLite.h"

class WorldGen {
	FastNoiseLite noiseSampler;


	static WorldGen* _Instance;
public:
	WorldGen();

	static float SampleWorldHeight(int x, int z);
};
