#include "WorldGen.h"

#include <time.h>

#include "ShardGen.h"
#include "Biome.h"

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

	Init();
}

void WorldGen::SetSeed(int seed)
{
	_Instance->seed = seed;
	_Instance->Init();
}

int WorldGen::GetSeed() {
	return _Instance->seed;
}

void WorldGen::Init()
{
	//srand(time(NULL));
	//int seed = rand();
	//int seed = 1337;

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

	noiseSampler_HeightVal1 = FastNoiseLite(seed + 1);
	noiseSampler_HeightVal1.SetNoiseType(FastNoiseLite::NoiseType_Value);
	noiseSampler_HeightVal1.SetFrequency(1.f);

	noiseSampler_Caves1 = FastNoiseLite(seed);
	noiseSampler_Caves1.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noiseSampler_Caves1.SetFrequency(0.035f);
	noiseSampler_Caves1.SetFractalType(FastNoiseLite::FractalType_FBm);

	//noiseSampler_CavesTunnels = FastNoiseLite(seed);
	//noiseSampler_CavesTunnels.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	//noiseSampler_CavesTunnels.SetFrequency(0.03f);
	////noiseSampler_CavesTunnels.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction_EuclideanSq);
	////noiseSampler_CavesTunnels.SetCellularReturnType(FastNoiseLite::CellularReturnType_Distance2Div);

	//noiseSampler_CavesTunnels.SetFractalType(FastNoiseLite::FractalType_PingPong);
	//noiseSampler_CavesTunnels.SetFractalOctaves(1);
	//noiseSampler_CavesTunnels.SetFractalPingPongStrength(1.95f);

	//noiseSampler_Sky_Top = FastNoiseLite(seed);
	//noiseSampler_Sky_Top.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
	//noiseSampler_Sky_Top.SetFrequency(0.02f);
	//noiseSampler_Sky_Top.SetFractalType(FastNoiseLite::FractalType_FBm);

	//noiseSampler_Sky_Under = FastNoiseLite(seed+1);
	//noiseSampler_Sky_Under.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
	//noiseSampler_Sky_Under.SetFrequency(0.0375f);
	//noiseSampler_Sky_Under.SetFractalType(FastNoiseLite::FractalType_FBm);

	noiseSampler_Temperature = FastNoiseLite(seed + 1);
	noiseSampler_Temperature.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noiseSampler_Temperature.SetFrequency(0.0015f);
	noiseSampler_Temperature.SetFractalType(FastNoiseLite::FractalType_None);

	noiseSampler_Moisture = FastNoiseLite(seed + 2);
	noiseSampler_Moisture.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noiseSampler_Moisture.SetFrequency(0.0015f);
	noiseSampler_Moisture.SetFractalType(FastNoiseLite::FractalType_None);

	noiseSampler_Oceans = FastNoiseLite(seed + 1);
	noiseSampler_Oceans.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	noiseSampler_Oceans.SetFrequency(0.00175f);
	noiseSampler_Oceans.SetFractalType(FastNoiseLite::FractalType_None);

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

	noiseSampler_Rivers = FastNoiseLite(seed);
	//noiseSampler_Rivers.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
	//noiseSampler_Rivers.SetFrequency(0.01f);
	//noiseSampler_Rivers.SetDomainWarpType(FastNoiseLite::DomainWarpType_OpenSimplex2);
	//noiseSampler_Rivers.SetDomainWarpAmp(280.f);
	//noiseSampler_Rivers.SetFractalType(FastNoiseLite::FractalType_DomainWarpIndependent);
	//noiseSampler_Rivers.SetFractalOctaves(5);
	//noiseSampler_Rivers.SetFractalLacunarity(2.0f);
	//noiseSampler_Rivers.SetFractalGain(0.5f);
	//noiseSampler_Rivers.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
	//noiseSampler_Rivers.SetFrequency(0.01f);
	//noiseSampler_Rivers.SetFractalType(FastNoiseLite::FractalType_None);
	//noiseSampler_Rivers.set

	noiseSampler_CavesTunnelsN0 = FastNoiseLite(seed + 3);
	noiseSampler_CavesTunnelsN0.SetFrequency(0.01f);
	noiseSampler_CavesTunnelsN0.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	noiseSampler_CavesTunnelsN0.SetFractalType(FastNoiseLite::FractalType_FBm);

	noiseSampler_CavesTunnelsN1 = FastNoiseLite(seed + 4);
	noiseSampler_CavesTunnelsN1.SetFrequency(0.01f);
	noiseSampler_CavesTunnelsN1.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	noiseSampler_CavesTunnelsN1.SetFractalType(FastNoiseLite::FractalType_FBm);
	//noiseSampler_CavesTunnels.SetCellularReturnType(FastNoiseLite::CellularReturnType_Distance);



	noiseSampler_Cobbled = FastNoiseLite(seed);
	noiseSampler_Cobbled.SetFrequency(1.f);
	noiseSampler_Cobbled.SetNoiseType(FastNoiseLite::NoiseType_Value);
	noiseSampler_Cobbled.SetFractalType(FastNoiseLite::FractalType_None);

	noiseSampler_CobbledPresence = FastNoiseLite(seed);
	noiseSampler_CobbledPresence.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noiseSampler_CobbledPresence.SetFrequency(0.1f);
	noiseSampler_CobbledPresence.SetFractalType(FastNoiseLite::FractalType_None);
}

inline float clamp(float x, float minX, float maxX) {
	return min(max(x, minX), maxX);
}

inline float smoothstep(float edge0, float edge1, float x) {
	x = clamp((x - edge0) / (edge1 - edge0), 0.f, 1.f);
	return x * x * (3.0f - 2.0f * x);
}

inline float lerptab(float t, float a, float b) {
	return a + t * (b - a);
}

float WorldGen::SampleWorldHeight(const int& x, const int& z)
{
	float biomeConfidence = 1.0f;
	const Biome& biome = Biome::Get(SampleTemperature(x, z), SampleMoisture(x, z), SampleOceanMap(x,z), &biomeConfidence);
	float height = 0.0f;

	// Sample point
	Vector2 samp = Vector2((float)x, (float)z);
	samp /= 450.f; // Frequency

	// The basic non-mountainous heightmap
	float basicHeight = DeNormalizeNoise(layered_shard_noise(samp, _Instance->noiseSampler_HeightVal0, _Instance->noiseSampler_HeightVal1, 0.f, 4.f, 5)) * 30.f;
	basicHeight += 4;

	float privateHeight = basicHeight;

	switch(biome.noiseType) {
	case NOISETYPE_DEFAULT: break;
	case NOISETYPE_MOUNTAINS: {
		// Mountainous presence map (not height sample)
		float mountains = NormalizeNoise(_Instance->noiseSampler_Mountains.GetNoise((float)x, (float)z));
		mountains = clamp(smoothstep(0.275f, 1.f, mountains), 0.f, 1.f); // Smoothstep and clamp to reduce spots

		float mountainHeightOffset = lerptab(0.95f, 0.f, 22.f);
		// Sample raw height before offset base height
		float rawMountainSample = DeNormalizeNoise(layered_shard_noise(samp, _Instance->noiseSampler_HeightVal0, _Instance->noiseSampler_HeightVal1, 1.f, 4.f, 5));
		// Clamp height and then offset by base height
		float mountainHeight = (clamp(rawMountainSample, 0, 3.f) * 30.f) - mountainHeightOffset;

		privateHeight = lerptab(mountains, basicHeight, mountainHeight);
		}
		break;
	case NOISETYPE_OCEAN: 
		privateHeight = (basicHeight / 2.f) - 30;
		break;
	case NOISETYPE_BEACH:
		privateHeight = basicHeight / 3.0f;
		break;
	case NOISETYPE_PLATEAU: {
		float height = round((basicHeight * ((basicHeight > 0) ? 7.f : -2.f)) / 7.0f);
		privateHeight = (float)fmin(((height > 5) ? (height * 7.f)-16 : height * 4.0f), 78);

		}	
		break;
	}

	return lerp(basicHeight, privateHeight, biomeConfidence);

	return height;
}

float WorldGen::SampleWorldSteepness(int x, int z)
{
	float px = SampleWorldHeight(x + 1, z);
	float nx = SampleWorldHeight(x - 1, z);
	float pz = SampleWorldHeight(x, z + 1);
	float nz = SampleWorldHeight(x, z - 1);

	float dX = abs(px - nx);
	float dZ = abs(pz - nz);

	return max(dX,dZ);
}

float WorldGen::SampleOceanMap(int x, int z) {
	return NormalizeNoise(_Instance->noiseSampler_Oceans.GetNoise((float)x, (float)z));
}

float WorldGen::SampleTemperature(const int& x, const int& z) {
	return NormalizeNoise(_Instance->noiseSampler_Temperature.GetNoise((float)x, (float)z) * 1.08f);
}

float WorldGen::SampleMoisture(const int& x, const int& z) {
	return NormalizeNoise(_Instance->noiseSampler_Moisture.GetNoise((float)x, (float)z) * 1.08f);
}

BlockID WorldGen::GetBlockAt(const int& x, const int& y, const int& z) {
	float heightSample = SampleWorldHeight(x, z);
	float temperatureSample = SampleTemperature(x, z);
	float moistureSample = SampleMoisture(x, z);
	float oceanSample = SampleOceanMap(x, z);



	return GetBlockGivenHeight(x, y, z, static_cast<int>(heightSample), Biome::Get(temperatureSample, moistureSample, oceanSample), moistureSample);
}

bool WorldGen::IsBlockCave(const int& x, const int& y, const int& z) {
	const float tunnelIntersection = 0.08f;
	float sample = _Instance->noiseSampler_Caves1.GetNoise(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));

#if 0
	float sample0 = abs(_Instance->noiseSampler_CavesTunnelsN0.GetNoise((float)x, (float)y * 1.2f, (float)z));
	float sample1 = abs(_Instance->noiseSampler_CavesTunnelsN1.GetNoise((float)x, (float)y * 1.2f, (float)z));
#else
	float sample0 = 1.f;
	float sample1 = 1.f;
#endif
	//float sampleTunnels = _Instance->noiseSampler_CavesTunnels.GetNoise(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));

	//return sample > 
	//return sampleTunnels < -0.8f;
	//return (sample > 0.25f) || (sampleTunnels > -0.1f);
	//return (sample > 0.25f) || (sampleTunnels > 0.4f && sampleTunnels < 0.6f);
	//return (sample > 0.25f) || (abs(sampleTunnels) > 0.4f && abs(sampleTunnels) < 0.6f);

	//return sample > 0.25f;
	return (sample0 < tunnelIntersection && sample1 < tunnelIntersection) || sample > 0.25f;
}


// This is also slow, need to think hmmm
// just thought of a way

// sample height noise at chunk origin
// if the chunk is so high (or maybe so below) the height, dont bother generating the chunk

BlockID WorldGen::GetBlockGivenHeight(const int& x, const int& y, const int& z, const int& heightSample, const Biome& biome, const float moisture)
{
	const int SEA_LEVEL = 0;

	//if(y <= heightSample) return biome.testBlock;
	if(y <= heightSample) {
		bool isInCave = IsBlockCave(x, y, z);
		if(isInCave) {
			if(y < -140) return LAVA;
			else return AIR;
		}

		if(y >= heightSample - 2) {
			float steepness = SampleWorldSteepness(x, z);

			if(steepness < biome.biomeSurface.maxTopSteepness) {
				if(y == heightSample) {
					if(y < SEA_LEVEL) return biome.biomeSurface.sea_floor;
					return biome.biomeSurface.top;
				}
				
				return biome.biomeSurface.mid;
			}
		}
		
		{ // stone
			BlockID stoneType = biome.biomeSurface.foundation;

			if(y <= -80 + (_Instance->noiseSampler_CobbledPresence.GetNoise((float)x, (float)z)*2.f)) stoneType = BLACKSTONE;

			

			// Value represents rarity
			float valueSample = _Instance->noiseSampler_treeValue.GetNoise((float)x, (float)y, (float)z);
			float distSample = _Instance->noiseSampler_treeDist.GetNoise((float)x, (float)y, (float)z);

			if(valueSample < -0.85f && distSample < -0.9f) {
				switch(stoneType) {
				case BLACKSTONE: return BLACK_COAL_ORE;
				case GRANITE: return GRANITE_COAL_ORE;
				default: return COAL_ORE;
				}
			}
			if(valueSample < -0.7f && distSample < -0.95f) {
				switch(stoneType) {
				case BLACKSTONE: return BLACK_COPPER_ORE;
				case GRANITE: return GRANITE_COPPER_ORE;
				default: return COPPER_ORE;
				}
			}
			if(y < -30 && valueSample < -0.55f && distSample < -0.96f) {
				switch(stoneType) {
				case BLACKSTONE: return BLACK_GOLD_ORE;
				case GRANITE: return GRANITE_GOLD_ORE;
				default: return GOLD_ORE;
				}
			}
			if(y < -45 && valueSample < -0.45f && distSample < -0.97f) {
				switch(stoneType) {
				case BLACKSTONE: return BLACK_AMETHYST_ORE;
				case GRANITE: return GRANITE_AMETHYST_ORE;
				default: return AMETHYST_ORE;
				}
			}
			if(y < -61 && valueSample < -0.35f && distSample < -0.97f) {
				switch(stoneType) {
				case BLACKSTONE: return BLACK_TITANIUM_ORE;
				case GRANITE: return GRANITE_TITANIUM_ORE;
				default: return TITANIUM_ORE;
				}
			}

			float cobbledValue = _Instance->noiseSampler_Cobbled.GetNoise((float)x, (float)y, (float)z);
			float cobbledPresence = _Instance->noiseSampler_CobbledPresence.GetNoise((float)x, (float)y, (float)z);

			if(cobbledValue > 0.35f && cobbledPresence > 0.2f) {
				switch(stoneType) {
				case BLACKSTONE: stoneType = BLACK_COBBLESTONE; break;
				case GRANITE: stoneType = SANDSTONE; break;
				case STONE: stoneType = COBBLESTONE; break;
				}
			}

			return stoneType;
		}
		
	}


	if(y <= SEA_LEVEL) return WATER;

	if(biome.treeWeights.size() > 0 && heightSample >= SEA_LEVEL) {
		const float moistureSlope = moisture / 1.15f;
		const float treeDensity = clamp(moistureSlope * moistureSlope * moistureSlope + 0.1f, 0.f, 1.f);

		if(_Instance->noiseSampler_treeValue.GetNoise((float)x, (float)z) < -(0.9f - treeDensity)) {


			if(y > heightSample && y < heightSample + 9) {
				float distSamp = _Instance->noiseSampler_treeDist.GetNoise((float)x, (float)z);

				if(distSamp < -0.989f && y < heightSample + 6) {
					return biome.treeWeights[0].first.wood;
				}

				if(distSamp < -0.9f && y > heightSample + 3) {
					return biome.treeWeights[0].first.leaves;
				}
			}
		}
	}

	if((y == heightSample + 1 || y <= SEA_LEVEL + 4) && biome.biomeFoliage.foliageWeights.size() > 0 && !IsBlockCave(x, y - 1, z)) {
		float foliageValue = _Instance->noiseSampler_Cobbled.GetNoise((float)x, (float)z);
		float foliagePresence = _Instance->noiseSampler_CobbledPresence.GetNoise((float)x, (float)z);


		if(foliagePresence > biome.biomeFoliage.foliagePresence) {
			if(heightSample <= SEA_LEVEL && y <= SEA_LEVEL + 4 && foliageValue > 0.95f && heightSample>=SEA_LEVEL) return SUGAR_CANE;
			
			if(y == heightSample + 1)
			return Utility::WeightedRandomPick<BlockID>(biome.biomeFoliage.foliageWeights, foliageValue);
		}
	}

	return AIR;

	//const Biome& biome = Biome::Get(tempSample, moistSample);


	//if(heightSample >= SEA_LEVEL) {
	//	const float moistureSlope = moisture / 1.15f;
	//	const float treeDensity = clamp(moistureSlope * moistureSlope * moistureSlope + 0.1f, 0.f, 1.f);

	//	if(_Instance->noiseSampler_treeValue.GetNoise((float)x, (float)z) < -(0.9f - treeDensity)) {


	//		if(y > heightSample && y < heightSample + 9) {
	//			float distSamp = _Instance->noiseSampler_treeDist.GetNoise((float)x, (float)z);

	//			if(distSamp < -0.989f && y < heightSample + 6) {
	//				return biome.wood;
	//			}

	//			if(distSamp < -0.9f && y > heightSample + 3) {
	//				return biome.leaves;
	//			}
	//		}

	//	}
	//}

	//if(y > heightSample) {
	//	if(y < SEA_LEVEL - 1) return WATER;

	//	//if(biome.hasFoliage) {

	//	//	if(y == heightSample + 1) {
	//	//		float grassValue = _Instance->noiseSampler_Cobbled.GetNoise((float)x, (float)z);
	//	//		float grassPresence = _Instance->noiseSampler_CobbledPresence.GetNoise((float)x, (float)z);
	//	//		if(grassPresence > 0.2f) {
	//	//			if(grassValue > 0.9f) return ROSE;
	//	//			if(grassValue > 0.8f) return DANDELION;
	//	//			if(grassValue > 0.3f ) return TALL_GRASS;
	//	//		}
	//	//	}
	//	//}

	//	return AIR;
	//}

	//if(y == heightSample) {
	//	if(y < SEA_LEVEL) {
	//		if(y >= SEA_LEVEL - 2) return biome.shore;
	//		return biome.waterBed;
	//	}
	//	bool isInCave = IsBlockCave(x, y, z);
	//	if(isInCave) return AIR;
	//	return biome.surface;
	//}

	//if(y == heightSample - 1) { // Just below the surface
	//	if(y < SEA_LEVEL) return biome.sandTypeBottom;
	//	bool isInCave = IsBlockCave(x, y, z);
	//	if(isInCave) return AIR;
	//	return biome.earthTop;

	//}

	//if(y == heightSample - 2) { // Bottom of crust
	//	if(y < SEA_LEVEL) return biome.clayType;
	//	bool isInCave = IsBlockCave(x, y, z);
	//	if(isInCave) return AIR;
	//	return biome.earthBottom;
	//}



	//bool isInCave = IsBlockCave(x, y, z);
	//if(isInCave) {
	//	if(y < -140) return LAVA;
	//	else return AIR;
	//}

	//BlockID stoneType = biome.stone;

	//if(y <= -80 + (_Instance->noiseSampler_CobbledPresence.GetNoise((float)x, (float)z)*2.f)) stoneType = BLACKSTONE;

	//

	//// Value represents rarity
	//float valueSample = _Instance->noiseSampler_treeValue.GetNoise((float)x, (float)y, (float)z);
	//float distSample = _Instance->noiseSampler_treeDist.GetNoise((float)x, (float)y, (float)z);

	//if(valueSample < -0.85f && distSample < -0.9f) {
	//	return static_cast<BlockID>(COAL_ORE + ((stoneType == BLACKSTONE) * 6));
	//}
	//if(valueSample < -0.7f && distSample < -0.95f) {
	//	return static_cast<BlockID>(COPPER_ORE + ((stoneType == BLACKSTONE) * 6));
	//}
	//if(y < -30 && valueSample < -0.55f && distSample < -0.96f) {
	//	return static_cast<BlockID>(GOLD_ORE + ((stoneType == BLACKSTONE) * 6));
	//}
	//if(y < -45 && valueSample < -0.45f && distSample < -0.97f) {
	//	return static_cast<BlockID>(AMETHYST_ORE + ((stoneType == BLACKSTONE) * 6));
	//}
	//if(y < -61 && valueSample < -0.35f && distSample < -0.97f) {
	//	return static_cast<BlockID>(TITANIUM_ORE + ((stoneType == BLACKSTONE) * 6));
	//}

	//float cobbledValue = _Instance->noiseSampler_Cobbled.GetNoise((float)x, (float)y, (float)z);
	//float cobbledPresence = _Instance->noiseSampler_CobbledPresence.GetNoise((float)x, (float)y, (float)z);

	//if(cobbledValue > 0.35f && cobbledPresence > 0.2f) {
	//	switch(stoneType) {
	//	case BLACKSTONE: stoneType = BLACK_COBBLESTONE; break;
	//	case STONE: stoneType = COBBLESTONE; break;
	//	}
	//}

	//return stoneType;
}