#include "WorldGen.h"

#include <time.h>
#include "ShardGen.h"

WorldGen* WorldGen::_Instance = new WorldGen();

float NormalizeNoise(const float& noise) {
	return (noise + 1.0f) / 2.0f;
}
float DeNormalizeNoise(const float& noise) {
	return (noise * 2.0f) - 1.0f;
}

WorldGen::WorldGen()
{
	this->_Instance = this;

	//srand(time(NULL));
	//int seed = rand();
	int seed = 69;

	noiseSampler_Height1 = FastNoiseLite(seed);
	noiseSampler_Height1.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noiseSampler_Height1.SetFrequency(0.02f);
	noiseSampler_Height1.SetFractalType(FastNoiseLite::FractalType_FBm);

	noiseSampler_Height1.SetFractalOctaves(3);
	noiseSampler_Height1.SetFractalLacunarity(2.03f);
	noiseSampler_Height1.SetFractalGain(0.6f);

	noiseSampler_Caves1 = FastNoiseLite(seed);
	noiseSampler_Caves1.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noiseSampler_Caves1.SetFrequency(0.035f);
	noiseSampler_Caves1.SetFractalType(FastNoiseLite::FractalType_FBm);

	noiseSampler_Sky_Top = FastNoiseLite(seed);
	noiseSampler_Sky_Top.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
	noiseSampler_Sky_Top.SetFrequency(0.02f);
	noiseSampler_Sky_Top.SetFractalType(FastNoiseLite::FractalType_FBm);

	noiseSampler_Sky_Under = FastNoiseLite(seed+1);
	noiseSampler_Sky_Under.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
	noiseSampler_Sky_Under.SetFrequency(0.0375f);
	noiseSampler_Sky_Under.SetFractalType(FastNoiseLite::FractalType_FBm);

	noiseSampler_Temperature = FastNoiseLite(seed + 1);
	noiseSampler_Temperature.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noiseSampler_Temperature.SetFrequency(0.005f);
	noiseSampler_Temperature.SetFractalType(FastNoiseLite::FractalType_None);

	noiseSampler_Moisture = FastNoiseLite(seed + 2);
	noiseSampler_Moisture.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noiseSampler_Moisture.SetFrequency(0.005f);
	noiseSampler_Moisture.SetFractalType(FastNoiseLite::FractalType_None);
}

inline float clamp(float x, float minX, float maxX) {
	return min(max(x, minX), maxX);
}

inline float smoothstep(float edge0, float edge1, float x) {
	x = clamp((x - edge0) / (edge1 - edge0), 0.f, 1.f);
	return x * x * (3.0f - 2.0f * x);
}

float WorldGen::SampleWorldHeight(const int& x, const int& z)
{
#if 0
	float rawNoiseSample = _Instance->noiseSampler_Height1.GetNoise((float)x, (float)z);
	//rawNoiseSample = DeNormalizeNoise(smoothstep(0.2f, 0.8f, NormalizeNoise(rawNoiseSample)));

#else
	Vector2 samp = Vector2((float)x, (float)z);
	samp /= 450.f;
	float rawNoiseSample = DeNormalizeNoise( layered_shard_noise(samp, 4.f, 5) );
#endif

	return rawNoiseSample * 30.f;
}

float WorldGen::SampleTemperature(const int& x, const int& z) {
	return NormalizeNoise(_Instance->noiseSampler_Temperature.GetNoise((float)x, (float)z));
}

float WorldGen::SampleMoisture(const int& x, const int& z) {
	return NormalizeNoise(_Instance->noiseSampler_Moisture.GetNoise((float)x, (float)z));
}

BlockID WorldGen::GetBlockAt(const int& x, const int& y, const int& z) {
	float heightSample = SampleWorldHeight(x, z);
	float temperatureSample = SampleTemperature(x, z);
	float moistureSample = SampleMoisture(x, z);
	return GetBlockGivenHeight(x, y, z, static_cast<int>(heightSample), temperatureSample, moistureSample);
}

bool WorldGen::IsBlockCave(const int& x, const int& y, const int& z) {
	return _Instance->noiseSampler_Caves1.GetNoise(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)) > 0.25f;
}



BlockID WorldGen::GetBlockGivenHeight(const int& x, const int& y, const int& z, const int& heightSample, const float& tempSample, const float& moistSample)
{
	const int SEA_LEVEL = 0;
	const int SKY_LEVEL = 200;

	//const bool decisions[7] = {
	//	y > heightSample, //		above ground
	//	y == heightSample, //		is surface
	//	y == heightSample - 1, //	just below surface
	//	y == heightSample - 2, //	bottom of surface layer

	//	y < SEA_LEVEL, //			4	below sea level
	//	y > SKY_LEVEL, //			2	above sky level

	//	IsBlockCave(x, y, z), //	1	in cave
	//};

	//todo: this doesnt feel right, there HAS HAS HAS to be a better way to do this
	// I know the bunch of if statements look weird, but if we follow it like the computer does, its actually faster than precomputing the individual conditions

	// maybe tree structure?

	if(y > heightSample) {
		if(y < SEA_LEVEL-1) return WATER;
		//if(y > SKY_LEVEL) {
		//	int skyTop = static_cast<int>((NormalizeNoise(_Instance->noiseSampler_Sky_Top.GetNoise((float)x, (float)z)) * 30.0f) + SKY_LEVEL);
		//	int skyUnder = static_cast<int>((NormalizeNoise(_Instance->noiseSampler_Sky_Under.GetNoise((float)x, (float)z) - 0.5f) * 70.0f) + SKY_LEVEL);

		//	if(y <= skyTop && y > skyUnder) {
		//		if(y == skyTop) return GRASS;
		//		if(y == skyTop - 1) return DIRT;
		//		if(y == skyTop - 2) return DIRT;
		//		return STONE;
		//	}
		//	return AIR;
		//}
		return AIR;
	}


	if(y == heightSample) {
		if(y < SEA_LEVEL) return SAND;
		bool isInCave = IsBlockCave(x, y, z);
		if(isInCave) return AIR;
		return GRASS;
	}

	if(y == heightSample - 1) { // Just below the surface
		if(y < SEA_LEVEL) return SAND;
		bool isInCave = IsBlockCave(x, y, z);
		if(isInCave) return AIR;
		return DIRT;
		
	}

	if(y == heightSample - 2) { // Bottom of crust
		if(y < SEA_LEVEL) return CLAY;
		bool isInCave = IsBlockCave(x, y, z);
		if(isInCave) return AIR;
		return DIRT;
	}

	bool isInCave = IsBlockCave(x, y, z);
	if(isInCave) return AIR;
	return STONE;
}