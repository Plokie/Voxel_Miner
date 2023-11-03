#include "WorldGen.h"

WorldGen* WorldGen::_Instance = new WorldGen();

WorldGen::WorldGen()
{
	this->_Instance = this;

	noiseSampler_Height1 = FastNoiseLite(1337);
	noiseSampler_Height1.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noiseSampler_Height1.SetFrequency(0.02f);
	noiseSampler_Height1.SetFractalType(FastNoiseLite::FractalType_FBm);

	noiseSampler_Height1.SetFractalOctaves(3);
	noiseSampler_Height1.SetFractalLacunarity(2.03f);
	noiseSampler_Height1.SetFractalGain(0.6f);

	noiseSampler_Caves1 = FastNoiseLite(1337);
	noiseSampler_Caves1.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noiseSampler_Caves1.SetFrequency(0.035f);
	noiseSampler_Caves1.SetFractalType(FastNoiseLite::FractalType_FBm);
	//noiseSampler_Caves1.SetFractalOctaves(1);
	//noiseSampler_Caves1.SetFractalLacunarity(1.99f);
	//noiseSampler_Caves1.SetFractalGain(0.5f);
	//noiseSampler_Caves1.SetFractalWeightedStrength(0.0f);
	//noiseSampler_Caves1.SetFractalPingPongStrength(1.25f);
	//noiseSampler_Caves1.SetCellularReturnType(FastNoiseLite::CellularReturnType_Distance2Div);
	//noiseSampler_Caves1.SetFractalType(FastNoiseLite::FractalType_None);
	//noiseSampler_Caves1.SetDomainWarpType(FastNoiseLite::DomainWarpType_BasicGrid);
	//noiseSampler_Caves1.SetDomainWarpAmp(161.0f);
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

	/*if(y < SEA_LEVEL) return GRASS;
	else return AIR;*/

	//todo: this doesnt feel right, there HAS HAS HAS to be a better way to do this
	// I know the bunch of if statements look weird, but if we follow it like the computer does, its actually faster than precomputing the individual conditions

	if(y <= heightSample) { // If the y is within the main heightmap

		if(y == heightSample) { // Top surface
			if(y < SEA_LEVEL) {
				return SAND;
			}
			else {
				bool isInCave = _Instance->noiseSampler_Caves1.GetNoise((float)x, (float)y, (float)z) > 0.25f;
				if(isInCave) {
					return AIR;
				}
				else {
					return GRASS;
				}
			}
		}

		if(y == heightSample - 1) { // Just below the surface
			if(y < SEA_LEVEL) {
				return SAND;
			}
			else {
				bool isInCave = _Instance->noiseSampler_Caves1.GetNoise((float)x, (float)y, (float)z) > 0.25f;
				if(isInCave) {
					return AIR;
				}
				else {
					return DIRT;
				}
			}
		}

		if(y == heightSample - 2) { // Bottom of crust
			if(y < SEA_LEVEL) {
				return CLAY;
			}
			else {
				bool isInCave = _Instance->noiseSampler_Caves1.GetNoise((float)x, (float)y, (float)z) > 0.25f;
				if(isInCave) {
					return AIR;
				}
				else {
					return DIRT;
				}
			}
		}
		bool isInCave = _Instance->noiseSampler_Caves1.GetNoise((float)x, (float)y, (float)z) > 0.25f;
		if(isInCave) {
			return AIR;
		}
		else return STONE;
	}
	else { //If the y is above ground
		if(y < SEA_LEVEL) {
			return WATER;
		}
		return AIR;
	}


	/*if() {
		return BlockID::AIR;
	}

	if(y > heightSample && y <= 0) {
		return BlockID::WATER;
	}
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
	}*/
}
