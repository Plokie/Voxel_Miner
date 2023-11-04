#include "WorldGen.h"

WorldGen* WorldGen::_Instance = new WorldGen();

WorldGen::WorldGen()
{
	this->_Instance = this;

	noiseSampler_Height1 = FastNoiseLite(69);
	noiseSampler_Height1.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noiseSampler_Height1.SetFrequency(0.02f);
	noiseSampler_Height1.SetFractalType(FastNoiseLite::FractalType_FBm);

	noiseSampler_Height1.SetFractalOctaves(3);
	noiseSampler_Height1.SetFractalLacunarity(2.03f);
	noiseSampler_Height1.SetFractalGain(0.6f);

	noiseSampler_Caves1 = FastNoiseLite(69);
	noiseSampler_Caves1.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noiseSampler_Caves1.SetFrequency(0.035f);
	noiseSampler_Caves1.SetFractalType(FastNoiseLite::FractalType_FBm);
}

float WorldGen::SampleWorldHeight(int x, int z)
{
	float rawNoiseSample = _Instance->noiseSampler_Height1.GetNoise((float)x, (float)z);


	return rawNoiseSample * 30.f;
}

BlockID WorldGen::GetBlockAt(int x, int y, int z)
{
	float heightSample = SampleWorldHeight(x, z);
	return GetBlockGivenHeight(x, y, z, static_cast<int>(heightSample));
}

BlockID WorldGen::GetBlockGivenHeight(int x, int y, int z, int heightSample)
{
	const int SEA_LEVEL = 0;

	//todo: this doesnt feel right, there HAS HAS HAS to be a better way to do this
	// I know the bunch of if statements look weird, but if we follow it like the computer does, its actually faster than precomputing the individual conditions

	// maybe tree structure?
	
	if(y > heightSample) {
		if(y < SEA_LEVEL) return WATER;
		return AIR;
	}

	if(y == heightSample) {
		if(y < SEA_LEVEL) return SAND;
		else {
			bool isInCave = _Instance->noiseSampler_Caves1.GetNoise((float)x, (float)y, (float)z) > 0.25f;
			if(isInCave)return AIR;
			return GRASS;
		}
	}

	if(y == heightSample - 1) { // Just below the surface
		if(y < SEA_LEVEL) return SAND;
		else {
			bool isInCave = _Instance->noiseSampler_Caves1.GetNoise((float)x, (float)y, (float)z) > 0.25f;
			if(isInCave) return AIR;
			return DIRT;
		}
	}

	if(y == heightSample - 2) { // Bottom of crust
		if(y < SEA_LEVEL) return CLAY;
		else {
			bool isInCave = _Instance->noiseSampler_Caves1.GetNoise((float)x, (float)y, (float)z) > 0.25f;
			if(isInCave) return AIR;
			return DIRT;
		}
	}

	bool isInCave = _Instance->noiseSampler_Caves1.GetNoise((float)x, (float)y, (float)z) > 0.25f;
	if(isInCave) return AIR;
	return STONE;
}
