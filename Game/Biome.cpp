#include "Biome.h"

#include "../Engine/MathUtil.h"

inline float clamp(float x, float minX, float maxX) {
	return min(max(x, minX), maxX);
}

inline float smoothstep(float edge0, float edge1, float x) {
	x = clamp((x - edge0) / (edge1 - edge0), 0.f, 1.f);
	return x * x * (3.0f - 2.0f * x);
}

const Biome& Biome::Get(float temperature, float moisture, float ocean, float* confidence)
{
	const float beachStart = 0.55f;
	const float oceanStart = 0.65f;

	if(ocean > oceanStart) {
		if(confidence)
			*confidence = clamp((ocean - oceanStart) / 0.2f, 0.f, 1.f);

		return Biome::def[BIOME_OCEAN];
	}
	if(ocean > beachStart) {
		if(confidence)
			*confidence = clamp(((float)fmin(ocean - beachStart, oceanStart - ocean)) / 0.05f, 0.f, 1.f);

		return Biome::def[BIOME_BEACH];
	}

	Vector3 point = Vector3(moisture, temperature, 0.f);
	for(auto& p : Biome::range) {
		if(p.second.IsPointWithin(point)) {

			if(confidence != nullptr) {
				Vector3 min, max;
				p.second.GetMinMax(&min, &max);

				Vector3 distance;
				distance.x = (float)fmax(min.x - point.x, point.x - max.x);
				distance.y = (float)fmax(min.y - point.y, point.y - max.y);
				distance.z = ocean - beachStart;

				float distToEdge = (float)fmin(abs(distance.x), fmin(abs(distance.y), abs(distance.z)));
				const float edgePad = 0.02f;

				*confidence = clamp(distToEdge / edgePad, 0.f, 1.f);
			}



			return Biome::def[p.first];
		}
	}
	return Biome::def[BIOME_GRASSLANDS];
}


map<BiomeID, Biome> Biome::def = {
	{BiomeID::BIOME_OCEAN, {
		"Ocean", NOISETYPE_OCEAN, 
		{ // Biome surface
			GRAVEL, //top
			CLAY, //earth
			STONE, //foundation
			3.f, //min steepness grass
			GRAVEL //water floor
		},
		{ // vector of <tree def : weight>s
		},
		{ //Biome foliage
			//1.0f, //density
			//{ // foliage blocks : weights
			//	{TALL_GRASS, 1.0f}
			//}
		}
	}},
	{BiomeID::BIOME_BEACH, {
		"Beach", NOISETYPE_BEACH,
		{ // Biome surface
			SAND, //top
			SAND, //earth
			STONE, //foundation
			15.f, //min steepness top
			GRAVEL //water floor
		},
		{ // vector of <tree def : weight>s
		},
		{ //Biome foliage
		}
	}},
	{BiomeID::BIOME_GRASSLANDS, {
		"Grasslands", NOISETYPE_DEFAULT,
		{ // Biome surface
			GRASS, //top
			DIRT, //earth
			STONE, //foundation
			15.f, //min steepness grass
			GRAVEL //water floor
		},
		{ // vector of <tree def : weight>s
			{{OAK_LOG, OAK_LEAVES}, 1}
		},
		{ //Biome foliage
			0.2f, //presence
			{ // foliage blocks : weights
				{AIR, 15},
				{TALL_GRASS, 4},
				{ROSE, 1},
				{DANDELION, 1},
			}
		}
	}},
	{BiomeID::BIOME_MOUNTAINS, {
		"Mountains", NOISETYPE_MOUNTAINS,
		{ // Biome surface
			GRASS, //top
			DIRT, //earth
			STONE, //foundation
			3.f, //min steepness grass
			GRAVEL //water floor
		},
		{ // vector of <tree def : weight>s
			{{OAK_LOG, OAK_LEAVES}, 1}
		},
		{ //Biome foliage
			0.2f, //presence
			{ // foliage blocks : weights
				{AIR, 15},
				{TALL_GRASS, 4},
				{ROSE, 1},
				{DANDELION, 1},
			}
		}
	}},
	{BiomeID::BIOME_FOREST, {
		"Forest", NOISETYPE_DEFAULT,
		{ // Biome surface
			GRASS, //top
			DIRT, //earth
			STONE, //foundation
			15.f, //min steepness grass
			GRAVEL //water floor
		},
		{ // vector of <tree def : weight>s
			{{OAK_LOG, OAK_LEAVES}, 1}
		},
		{ //Biome foliage
			-0.8f, //presence
			{ // foliage blocks : weights
				{AIR, 15},
				{TALL_GRASS, 4},
				{ROSE, 3},
			}
		}
	}},
	{BiomeID::BIOME_FOREST_MOUNTAINS, {
		"Forest Mountains", NOISETYPE_MOUNTAINS,
		{ // Biome surface
			GRASS, //top
			DIRT, //earth
			STONE, //foundation
			3.f, //min steepness grass
			GRAVEL //water floor
		},
		{ // vector of <tree def : weight>s
			{{OAK_LOG, OAK_LEAVES}, 1}
		},
		{ //Biome foliage
			0.0f, //presence
			{ // foliage blocks : weights
				{AIR, 5},
				{TALL_GRASS, 1}
			}
		}
	}},
	{BiomeID::BIOME_TUNDRA, {
	"Tundra", NOISETYPE_DEFAULT,
	{ // Biome surface
		SNOW_GRASS, //top
		DIRT, //earth
		STONE, //foundation
		3.f, //min steepness grass
		GRAVEL //water floor
	},
	{ // vector of <tree def : weight>s
		//{{OAK_LOG, OAK_LEAVES}, 1}
	},
	{ //Biome foliage
		0.2f, //presence
		{ // foliage blocks : weights
			{AIR, 10},
			{TALL_GRASS, 1}
		}
	}
} },
	{BiomeID::BIOME_TUNDRA_MOUNTAINS, {
		"Tundra Mountains", NOISETYPE_MOUNTAINS,
		{ // Biome surface
			SNOW_GRASS, //top
			DIRT, //earth
			STONE, //foundation
			3.f, //min steepness grass
			GRAVEL //water floor
		},
		{ // vector of <tree def : weight>s
			//{{OAK_LOG, OAK_LEAVES}, 1}
		},
		{ //Biome foliage
			0.2f, //presence
			{ // foliage blocks : weights
				{AIR, 12},
				{TALL_GRASS, 1}
			}
		}
	}},
	{BiomeID::BIOME_SNOW, {
		"Snowy", NOISETYPE_DEFAULT,
		{ // Biome surface
			SNOW_GRASS, //top
			DIRT, //earth
			STONE, //foundation
			3.f, //min steepness grass
			GRAVEL //water floor
		},
		{ // vector of <tree def : weight>s
			{{OAK_LOG, OAK_LEAVES}, 1}
		},
		{ //Biome foliage
			0.2f, //presence
			{ // foliage blocks : weights
				{AIR, 8},
				{TALL_GRASS, 1}
			}
		}
	}},
	{BiomeID::BIOME_SNOW_MOUNTAINS, {
		"Snowy Mountains", NOISETYPE_MOUNTAINS,
		{ // Biome surface
			SNOW_GRASS, //top
			DIRT, //earth
			STONE, //foundation
			3.f, //min steepness grass
			GRAVEL //water floor
		},
		{ // vector of <tree def : weight>s
			{{OAK_LOG, OAK_LEAVES}, 1}
		},
		{ //Biome foliage
			0.2f, //presence
			{ // foliage blocks : weights
				{AIR, 9},
				{TALL_GRASS, 1}
			}
		}
	}},
	{BiomeID::BIOME_DESERT, {
		"Desert", NOISETYPE_DEFAULT,
		{ // Biome surface
			SAND, //top
			SANDSTONE, //earth
			GRANITE, //foundation
			3.f, //min steepness grass
			SAND //water floor
		},
		{ // vector of <tree def : weight>s
			{{CACTUS, AIR}, 1}
		},
		{ //Biome foliage
			0.2f, //presence
			{ // foliage blocks : weights
				{AIR, 20},
				{DEAD_BUSH, 5}
			}
		}
	}},
	{BiomeID::BIOME_DESERT_MOUNTAINS, {
		"Desert Mountains", NOISETYPE_MOUNTAINS,
		{ // Biome surface
			SAND, //top
			SANDSTONE, //earth
			GRANITE, //foundation
			3.f, //min steepness grass
			SAND //water floor
		},
		{ // vector of <tree def : weight>s
			{{CACTUS, AIR}, 1}
		},
		{ //Biome foliage
			0.2f, //presence
			{ // foliage blocks : weights
				{AIR, 20},
				{DEAD_BUSH, 5}
			}
		}
	}},
	{BiomeID::BIOME_TAIGA, {
		"Taiga", NOISETYPE_DEFAULT,
		{ // Biome surface
			TAIGA_GRASS, //top
			DIRT, //earth
			STONE, //foundation
			3.f, //min steepness grass
			GRAVEL //water floor
		},
		{ // vector of <tree def : weight>s
			{{SPRUCE_LOG, SPRUCE_LEAVES}, 1}
		},
		{ //Biome foliage
			0.2f,
			{ // foliage blocks : weights
				{AIR, 15},
				{TALL_GRASS, 4},
				{DANDELION, 3},
			}
		}
	}},
	{BiomeID::BIOME_TAIGA_MOUNTAINS, {
		"Taiga Mountains", NOISETYPE_MOUNTAINS,
		{ // Biome surface
			TAIGA_GRASS, //top
			DIRT, //earth
			STONE, //foundation
			3.f, //min steepness grass
			GRAVEL //water floor
		},
		{ // vector of <tree def : weight>s
			{{SPRUCE_LOG, SPRUCE_LEAVES}, 1}
		},
		{ //Biome foliage
			0.2f, //presence
			{ // foliage blocks : weights
				{AIR, 5},
				{TALL_GRASS, 1}
			}
		}
	}},
	{BiomeID::BIOME_CHERRY, {
		"Cherry", NOISETYPE_DEFAULT,
		{ // Biome surface
			CHERRY_GRASS, //top
			DIRT, //earth
			STONE, //foundation
			3.f, //min steepness grass
			GRAVEL //water floor
		},
		{ // vector of <tree def : weight>s
			{{CHERRY_LOG, CHERRY_LEAVES}, 1}
		},
		{ //Biome foliage
			0.2f, //presence
			{ // foliage blocks : weights
				{AIR, 5},
				{TALL_GRASS, 1}
			}
		}
	}},
	{BiomeID::BIOME_CHERRY_MOUNTAINS, {
		"Cherry Mountains", NOISETYPE_MOUNTAINS,
		{ // Biome surface
			CHERRY_GRASS, //top
			DIRT, //earth
			STONE, //foundation
			3.f, //min steepness grass
			GRAVEL //water floor
		},
		{ // vector of <tree def : weight>s
			{{CHERRY_LOG, CHERRY_LEAVES}, 1}
		},
		{ //Biome foliage
			0.2f, //presence
			{ // foliage blocks : weights
				{AIR, 5},
				{TALL_GRASS, 1}
			}
		}
	}},
	{BiomeID::BIOME_MAPLE, {
		"Maple", NOISETYPE_DEFAULT,
		{ // Biome surface
			MAPLE_GRASS, //top
			DIRT, //earth
			STONE, //foundation
			3.f, //min steepness grass
			GRAVEL //water floor
		},
		{ // vector of <tree def : weight>s
			{{BIRCH_LOG, BIRCH_LEAVES}, 1}
		},
		{ //Biome foliage
			0.2f, //presence
			{ // foliage blocks : weights
				{AIR, 5},
				{TALL_GRASS, 1}
			}
		}
	}},
	{BiomeID::BIOME_MAPLE_MOUNTAINS, {
		"Maple Mountains", NOISETYPE_MOUNTAINS,
		{ // Biome surface
			MAPLE_GRASS, //top
			DIRT, //earth
			STONE, //foundation
			3.f, //min steepness grass
			GRAVEL //water floor
		},
		{ // vector of <tree def : weight>s
			{{BIRCH_LOG, BIRCH_LEAVES}, 1}
		},
		{ //Biome foliage
			0.2f, //presence
			{ // foliage blocks : weights
				{AIR, 5},
				{TALL_GRASS, 1}
			}
		}
	}},
	{BiomeID::BIOME_GRANITE_VALLEY, {
		"Granite Valley", NOISETYPE_PLATEAU,
		{ // Biome surface
			CHERRY_GRASS, //top
			GRANITE, //earth
			GRANITE, //foundation
			0.5f, //min steepness grass
			GRAVEL //water floor
		},
		{ // vector of <tree def : weight>s
			{{OAK_LOG, AIR}, 1}
		},
		{ //Biome foliage
			0.2f, //presence
			{ // foliage blocks : weights
				{AIR, 20},
				{TALL_GRASS, 2},
				{DEAD_BUSH, 1},
			}
		}
	}},
	{BiomeID::BIOME_JUNGLE, {
		"(WIP) Jungle", NOISETYPE_DEFAULT,
		{ // Biome surface
			GRASS, //top
			DIRT, //earth
			STONE, //foundation
			15.f, //min steepness grass
			GRAVEL //water floor
		},
		{ // vector of <tree def : weight>s
			{{OAK_LOG, OAK_LEAVES}, 1}
		},
		{ //Biome foliage
			0.0f, //presence
			{ // foliage blocks : weights
				{AIR, 1},
				{TALL_GRASS, 1}
			}
		}
	}},
};

//// AABB lookup is based on bottom left to top right
//// X = Moisture 0.0f to 1.0f
//// Y = Temperature 0.0f to 1.0f
vector<pair<BiomeID, AABB>> Biome::range = {
	//{BiomeID::BIOME_GRASSLANDS, AABB::FromMinMax(
	//	{0.0f, 0.0f}, {1.0f, 0.5f}
	//)},
	{BiomeID::BIOME_MOUNTAINS, AABB::FromMinMax(
		{0.31f, 0.25f}, {0.52f, 0.44f}
	)},
	{BiomeID::BIOME_TUNDRA, AABB::FromMinMax(
		{0.0f, 0.0f}, {0.21f, 0.25f}
	)},
	{BiomeID::BIOME_TUNDRA_MOUNTAINS, AABB::FromMinMax(
		{0.21f, 0.0f}, {0.35f, 0.25f}
	)},
	{BiomeID::BIOME_SNOW, AABB::FromMinMax(
		{0.35f, 0.0f}, {1.0f, 0.15f}
	)},
	{BiomeID::BIOME_SNOW_MOUNTAINS, AABB::FromMinMax(
		{0.35f, 0.15f}, {0.63f, 0.25f}
	)},
	{BiomeID::BIOME_SNOW, AABB::FromMinMax(
		{0.35f, 0.0f}, {1.0f, 0.15f}
	)},
	{BiomeID::BIOME_TAIGA, AABB::FromMinMax(
		{0.63f, 0.15f}, {1.0f, 0.44f}
	)},
	{BiomeID::BIOME_TAIGA_MOUNTAINS, AABB::FromMinMax(
		{0.52f, 0.25f}, {0.63f, 0.44f}
	)},
	{BiomeID::BIOME_MAPLE, AABB::FromMinMax(
		{0.0f, 0.25f}, {0.20f, 0.44f}
	)},
	{BiomeID::BIOME_MAPLE_MOUNTAINS, AABB::FromMinMax(
		{0.20f, 0.25f}, {0.31f, 0.44f}
	)},
	{BiomeID::BIOME_FOREST, AABB::FromMinMax(
		{0.66f, 0.44f}, {0.84f, 0.78f}
	)},
	{BiomeID::BIOME_FOREST_MOUNTAINS, AABB::FromMinMax(
		{0.84f, 0.44f}, {1.0f, 0.78f}
	)},
	{BiomeID::BIOME_CHERRY, AABB::FromMinMax(
		{0.0f, 0.44f}, {0.10f, 0.67f}
	)},
	{BiomeID::BIOME_CHERRY_MOUNTAINS, AABB::FromMinMax(
		{0.10f, 0.44f}, {0.22f, 0.67f}
	)},
	{BiomeID::BIOME_CHERRY, AABB::FromMinMax(
		{0.22f, 0.44f}, {0.31f, 0.67f}
	)},
	{BiomeID::BIOME_GRANITE_VALLEY, AABB::FromMinMax(
		{0.0f, 0.67f}, {0.53f, 0.78f}
	)},
	{BiomeID::BIOME_GRANITE_VALLEY, AABB::FromMinMax(
		{0.0f, 0.60f}, {0.31f, 0.67f}
	)},
	{BiomeID::BIOME_DESERT, AABB::FromMinMax(
		{0.0f, 0.78f}, {0.53f, 1.0f}
	)},
	{BiomeID::BIOME_DESERT_MOUNTAINS, AABB::FromMinMax(
		{0.53f, 0.78f}, {0.69f, 1.0f}
	)},
	{BiomeID::BIOME_JUNGLE, AABB::FromMinMax(
		{0.69f, 0.78f}, {1.0f, 1.0f}
	)},
};

//
//map<BiomeID, Biome> Biome::def = {
//	{BiomeID::GRASSLANDS, {
//		"Grasslands",
//		GRASS,
//		DIRT,
//		DIRT,
//		STONE,
//
//		SAND, // Shore
//		SAND, // Water bed
//		SAND, // Sand under surface
//		CLAY, // Clay layer under sand
//
//		OAK_LOG,
//		OAK_LEAVES,
//		true
//	}},
//	{BiomeID::SNOW, {
//		"Snow",
//		SNOW_GRASS,
//		DIRT,
//		DIRT,
//		STONE,
//
//		SNOW_GRASS, // Shore
//		DIRT, // Water bed
//		DIRT, // Sand under surface
//		CLAY, // Clay layer under sand
//
//		SPRUCE_LOG,
//		SPRUCE_LEAVES,
//		false
//	}},
//	{BiomeID::DESERT, {
//		"Desert",
//		SAND,
//		SAND,
//		SAND,
//		STONE,
//
//		SAND, // Shore
//		SAND, // Water bed
//		SAND, // Sand under surface
//		CLAY, // Clay layer under sand
//
//		CACTUS,
//		AIR,
//		false
//	}},
//	{BiomeID::TAIGA, {
//		"Taiga",
//		TAIGA_GRASS,
//		DIRT,
//		DIRT,
//		STONE,
//
//		TAIGA_GRASS,
//		DIRT,
//		DIRT,
//		DIRT,
//
//		SPRUCE_LOG,
//		SPRUCE_LEAVES,
//		true
//	}},
//	{BiomeID::CHERRY, {
//		"Cherry",
//		CHERRY_GRASS,
//		DIRT,
//		DIRT,
//		STONE,
//
//		SAND,
//		SAND,
//		DIRT,
//		CLAY,
//
//		CHERRY_LOG,
//		CHERRY_LEAVES,
//		true
//	}},
//	{BiomeID::MAPLE, {
//		"Maple",
//		MAPLE_GRASS,
//		DIRT,
//		DIRT,
//		STONE,
//
//		MAPLE_GRASS,
//		GRAVEL,
//		DIRT,
//		CLAY,
//
//		BIRCH_LOG,
//		BIRCH_LEAVES,
//		true
//	}},
//	{BiomeID::GRANITE_VALLEY, {
//		"Granite Valley",
//		MAPLE_GRASS,
//		DIRT,
//		DIRT,
//		STONE,
//
//		MAPLE_GRASS,
//		GRAVEL,
//		DIRT,
//		CLAY,
//
//		BIRCH_LOG,
//		BIRCH_LEAVES,
//		true
//	}},
//};
//
//// AABB lookup is based on bottom left to top right
//// X = Moisture 0.0f to 1.0f
//// Y = Temperature 0.0f to 1.0f
//vector<pair<BiomeID, AABB>> Biome::range = {
//	//{WorldGen::BiomeID::SNOW, AABB({0.5f, 0.5f}, {0.5f, 0.5f})},
//	{	 BiomeID::SNOW, AABB::FromMinMax(
//		{0.0f, 0.0f}, {0.5f, 0.35f}
//	)},
//	{	 BiomeID::TAIGA, AABB::FromMinMax(
//		{0.5f, 0.0f}, {1.0f, 0.35f}
//	)},
//	{	 BiomeID::CHERRY, AABB::FromMinMax(
//		{0.6f, 0.5f}, {1.0f, 1.0f}
//	)},
//	{ BiomeID::DESERT, AABB::FromMinMax(
//		{0.0f, 0.51f}, {0.5f, 1.0f}
//	)},
//	{	 BiomeID::MAPLE, AABB::FromMinMax(
//		{0.35f, 0.5f}, {1.0f, 1.0f}
//	)},
//
//
//	//JUNGLE / RAINFOREST
//	/*{	 BiomeID::TAIGA, AABB::FromMinMax(
//		{0.6f, 0.66666f}, {1.0f, 1.0f}
//	)},*/
//
//
//	// GRASSLANDS is a default fallbakc when calling Get()
//	//{ BiomeID::GRASSLANDS, AABB( // Final fallback. Defaults to grasslands
//	//	{0.5f, 0.5f}, {0.5f, 0.5f}
//	//)},
//};


