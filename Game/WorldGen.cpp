#include "WorldGen.h"

#include <time.h>
#include "ShardGen.h"

WorldGen* WorldGen::_Instance = new WorldGen();

map<WorldGen::BiomeID, WorldGen::Biome> WorldGen::Biome::def = {  };


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
	int seed = 1337;

	//noiseSampler_Height1 = FastNoiseLite(seed);
	//noiseSampler_Height1.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	//noiseSampler_Height1.SetFrequency(0.02f);
	//noiseSampler_Height1.SetFractalType(FastNoiseLite::FractalType_FBm);
	// 
	//noiseSampler_Height1.SetFractalOctaves(3);
	//noiseSampler_Height1.SetFractalLacunarity(2.03f);
	//noiseSampler_Height1.SetFractalGain(0.6f);

	noiseSampler_HeightVal0 = FastNoiseLite(seed);
	noiseSampler_HeightVal0.SetNoiseType(FastNoiseLite::NoiseType_Value);
	noiseSampler_HeightVal0.SetFrequency(1.f);

	noiseSampler_HeightVal1 = FastNoiseLite(seed+1);
	noiseSampler_HeightVal1.SetNoiseType(FastNoiseLite::NoiseType_Value);
	noiseSampler_HeightVal1.SetFrequency(1.f);

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

	noiseSampler_Mountains = FastNoiseLite(seed);
	noiseSampler_Mountains.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	noiseSampler_Mountains.SetFrequency(0.004f);

	noiseSampler_treeDist = FastNoiseLite(seed);
	noiseSampler_treeDist.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
	noiseSampler_treeDist.SetFrequency(0.155f);
	noiseSampler_treeDist.SetCellularReturnType(FastNoiseLite::CellularReturnType_Distance);

	noiseSampler_treeValue = FastNoiseLite(seed);
	noiseSampler_treeValue.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
	noiseSampler_treeValue.SetFrequency(0.155f);
	noiseSampler_treeValue.SetCellularReturnType(FastNoiseLite::CellularReturnType_CellValue);
}

inline float clamp(float x, float minX, float maxX) {
	return min(max(x, minX), maxX);
}

inline float smoothstep(float edge0, float edge1, float x) {
	x = clamp((x - edge0) / (edge1 - edge0), 0.f, 1.f);
	return x * x * (3.0f - 2.0f * x);
}

inline float lerp(float t, float a, float b) {
	return a + t * (b - a);
}

float WorldGen::SampleWorldHeight(const int& x, const int& z)
{
#if 0 //Old world gen (keeping for legacy worlds)
	float rawNoiseSample = _Instance->noiseSampler_Height1.GetNoise((float)x, (float)z);
	//rawNoiseSample = DeNormalizeNoise(smoothstep(0.2f, 0.8f, NormalizeNoise(rawNoiseSample)));

	return rawNoiseSample * 30.f;
#else
	// Sample point
	Vector2 samp = Vector2((float)x, (float)z);
	samp /= 450.f; // Frequency

	// The basic non-mountainous heightmap
	float basicHeight = DeNormalizeNoise(layered_shard_noise(samp, _Instance->noiseSampler_HeightVal0, _Instance->noiseSampler_HeightVal1, 0.f, 4.f, 5)) * 30.f;

	// Mountainous presence map (not height sample)
	float mountains = NormalizeNoise(_Instance->noiseSampler_Mountains.GetNoise((float)x, (float)z));
	mountains = clamp(smoothstep(0.5f, 1.f, mountains), 0.f, 1.f); // Smoothstep and clamp to reduce spots

	// Since increasing the inital v value of shard_noise offsets the base height, we need to account for that here
	float mountainHeightOffset = lerp(mountains, 0.f, 22.f);
	// Sample raw height before offset base height
	float rawMountainSample = DeNormalizeNoise(layered_shard_noise(samp, _Instance->noiseSampler_HeightVal0, _Instance->noiseSampler_HeightVal1, 1.f, 4.f, 5));
	// Clamp height and then offset by base height
	float mountainHeight = (clamp(rawMountainSample, 0, 3.f) * 30.f) - mountainHeightOffset;

	return lerp(mountains, basicHeight, mountainHeight);
#endif
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


// This is also slow, need to think hmmm
// just thought of a way

// sample height noise at chunk origin
// if the chunk is so high (or maybe so below) the height, dont bother generating the chunk

BlockID WorldGen::GetBlockGivenHeight(const int& x, const int& y, const int& z, const int& heightSample, const float& tempSample, const float& moistSample)
{
	const int SEA_LEVEL = 0;
	const int SKY_LEVEL = 200;

	BlockID SURFACE = GRASS;
	BlockID EARTH_TOP = DIRT;
	BlockID EARTH_BOTTOM = DIRT;

	BlockID SAND_TYPE = SAND;
	BlockID CLAY_TYPE = CLAY;

	BlockID STONE_TYPE = STONE;

	//todo: this doesnt feel right, there HAS HAS HAS to be a better way to do this
	// I know the bunch of if statements look weird, but if we follow it like the computer does, its actually faster than precomputing the individual conditions

	// maybe tree structure?

	// this sucks, i just want something that gets wood in the world
	// todo: use terrain features
	
	if(heightSample >= SEA_LEVEL) {
		if(_Instance->noiseSampler_treeValue.GetNoise((float)x, (float)z) < -0.9f) {
			if(y > heightSample && y < heightSample + 9) {
				float distSamp = _Instance->noiseSampler_treeDist.GetNoise((float)x, (float)z);
				
				if(distSamp < -0.989f && y < heightSample + 6) {
					return OAK_LOG;
				}

				if(distSamp < -0.9f && y > heightSample + 3) {
					return OAK_LEAVES;
				}
			}

		}
	}

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
		if(y < SEA_LEVEL) return SAND_TYPE;
		bool isInCave = IsBlockCave(x, y, z);
		if(isInCave) return AIR;
		return SURFACE;
	}

	if(y == heightSample - 1) { // Just below the surface
		if(y < SEA_LEVEL) return SAND_TYPE;
		bool isInCave = IsBlockCave(x, y, z);
		if(isInCave) return AIR;
		return EARTH_TOP;
		
	}

	if(y == heightSample - 2) { // Bottom of crust
		if(y < SEA_LEVEL) return CLAY_TYPE;
		bool isInCave = IsBlockCave(x, y, z);
		if(isInCave) return AIR;
		return EARTH_BOTTOM;
	}

	bool isInCave = IsBlockCave(x, y, z);
	if (isInCave) return AIR;
	return STONE_TYPE;
}

//const WorldGen::Biome& WorldGen::Biome::Get(WorldGen::BiomeID id)
//{
//	// TODO: insert return statement here
//}
