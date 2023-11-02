#include "WorldGen.h"

WorldGen* WorldGen::_Instance = new WorldGen();

WorldGen::WorldGen()
{
	this->_Instance = this;

	noiseSampler = FastNoiseLite(1337);

	noiseSampler.SetSeed(1337);
	noiseSampler.SetFrequency(0.01f);

	noiseSampler.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
}

float WorldGen::SampleWorldHeight(int x, int z)
{
	float rawNoiseSample = _Instance->noiseSampler.GetNoise((float)x, (float)z);


	return rawNoiseSample * 20.f;
}
