#pragma once

//#include <Windows.h>
//#include <queue>
//#include "Chunk.h"
//
//#define LIGHT_LAYER_COUNT 2
//
////https://web.archive.org/web/20210429192404/https://www.seedofandromeda.com/blogs/29-fast-flood-fill-lighting-in-a-blocky-voxel-game-pt-1
//
//using namespace std;



//class ChunkLightData {
//private:
//	// like minecraft, light levels go from 0-15 (1-16) (guarantees lights dont go further than 2 chunks)
//	// each int contains 8 bits, 4 bits designated to each light layer (4 bits sky, 4 bits block)
//	// access block light with	level[x][y][z] & 0xF
//	// access sky light with	(level[x][y][z]>>4) & 0xF0
//	
//
//public:
//	Chunk* chunkParent = nullptr;
//
//	
//
//	ChunkLightData(Chunk* chunkParent);
//};