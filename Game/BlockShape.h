#pragma once

#include <map>
#include <vector>

#include "../Engine/MathUtil.h"
#include "../Engine/AABB.h"

using namespace std;

enum BlockShapeID {
	BLOCKSHAPE_BLOCK, BLOCKSHAPE_X, BLOCKSHAPE_SLAB
};

enum BlockShapeDirection : unsigned short {
	PX, NX, PY, NY, PZ, NZ, UNDEFINED
};



enum BlockShapeAxis : unsigned short {
	Y, X, Z //Y=0 as default
};

union BlockRotation {
	unsigned short raw;
	BlockShapeDirection direction;
	BlockShapeAxis axis;
};

struct BlockFace {
	Vector3 a, b, c, d; //4 vertices making a face
	Vector3 normal; //

	BlockFace(Vector3 a, Vector3 b, Vector3 c, Vector3 d, Vector3 normal) : a(a), b(b), c(c), d(d), normal(normal){}
};


//map of direction to faces
//map BlockShapeDirection : vector<BlockFace>


class BlockShape {
private:
	static map<tuple<int,int,int>, BlockShapeDirection> directionMap;

	map<BlockShapeDirection, vector<BlockFace>> _faceMap = {};
	map<BlockShapeDirection, bool> _obscures = {};
	vector<AABB> _aabbs = {};
	AABB maxAABB;
public:

	const AABB& GetMaxAABB() const { return maxAABB; }
	const vector<AABB>& GetAABBs() const { return _aabbs; }

	static BlockShapeDirection ToDirection(Vector3Int dir) {
		if(abs(dir.x) + abs(dir.y) + abs(dir.z) != 1) return UNDEFINED;
		return directionMap[dir];
	}

	static map<BlockShapeID, BlockShape> blockShapes;

	const vector<BlockFace>& GetFaces(BlockShapeDirection dir) const { return _faceMap.at(dir); }
	const bool ObscuresDirection(BlockShapeDirection dir) const { return _obscures.at(dir); }

	BlockShape() {
		_faceMap = {
			{PX, {}},
			{PY, {}},
			{PZ, {}},
			{NX, {}},
			{NY, {}},
			{NZ, {}},
			{UNDEFINED, {}},
		};
	}
	BlockShape(map<BlockShapeDirection, vector<BlockFace>> faceMap, map<BlockShapeDirection, bool> obscures, vector<AABB> aabbs);
};