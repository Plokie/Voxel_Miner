#include "WorldGen.h"

WorldGen* WorldGen::_Instance = new WorldGen();

WorldGen::WorldGen()
{
	this->_Instance = this;

	noiseSampler = FastNoiseLite(1337);

	noiseSampler.SetSeed(1337);
	noiseSampler.SetFrequency(0.02f);

	noiseSampler.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
}

float WorldGen::SampleWorldHeight(int x, int z)
{
	float rawNoiseSample = _Instance->noiseSampler.GetNoise((float)x, (float)z);


	return rawNoiseSample * 50.f;
}

BlockID WorldGen::GetBlockAt(int x, int y, int z)
{
	float heightSample = SampleWorldHeight(x, z);
	return GetBlockGivenHeight(x, y, z, heightSample);
}

BlockID WorldGen::GetBlockGivenHeight(int x, int y, int z, int heightSample)
{
	if(y < 2 && y<= heightSample) {
		return BlockID::SAND;
	}

	if (y == heightSample) {
		return BlockID::GRASS;
	}
	else if(y < heightSample - 3) {
		return BlockID::STONE;
	}
	else if(y < heightSample) {
		return BlockID::DIRT;
	}
	else {
		return BlockID::AIR;
	}
}
