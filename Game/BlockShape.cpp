#include "BlockShape.h"

map<tuple<int, int, int>, BlockShapeDirection> BlockShape::directionMap = {
	{{ 1, 0, 0}, PX},
	{{-1, 0, 0}, NX},
	{{ 0, 1, 0}, PY},
	{{ 0,-1, 0}, NY},
	{{ 0, 0, 1}, PZ},
	{{ 0, 0,-1}, NZ}
};

map<BlockShapeID, BlockShape> BlockShape::blockShapes = {
	{BLOCKSHAPE_BLOCK,{
		{// map to list of faces for each direction
			{PX, {
				BlockFace({1, 0, 0}, {1, 1, 0}, {1, 1, 1}, {1, 0, 1}, {1, 0, 0})
			}},
			{NX, {
				BlockFace({0, 0, 1}, {0, 1, 1}, {0, 1, 0}, {0, 0, 0}, {-1, 0, 0})
			}},
			{PY, {
				BlockFace({0, 1, 0}, {0, 1, 1}, {1, 1, 1}, {1, 1, 0}, {0, 1, 0})
			}},
			{NY, {
				BlockFace({0, 0, 1}, {0, 0, 0}, {1, 0, 0}, {1, 0, 1}, {0, -1, 0})
			}},
			{PZ, {
				BlockFace({1, 0, 1}, {1, 1, 1}, {0, 1, 1}, {0, 0, 1}, {0, 0, 1})
			}},
			{NZ, {
				BlockFace({0, 0, 0}, {0, 1, 0}, {1, 1, 0}, {1, 0, 0}, {0, 0, -1})
			}},
			{UNDEFINED, { // undefined when the face doesnt meet a traditional surface direction
			}}
		},
		{// obscures directions (for face-to-face culling)
			{PX, true},
			{NX, true},
			{PY, true},
			{NY, true},
			{PZ, true},
			{NZ, true},
		},
		{
			AABB::FromMinMax({0,0,0}, {1,1,1})
		}
	}},

	{BLOCKSHAPE_X,{
		{// map to list of faces for each direction
			{PX, {}},
			{NX, {}},
			{PY, {}},
			{NY, {}},
			{PZ, {}},
			{NZ, {}},
			{UNDEFINED, { // undefined when the face doesnt meet a traditional surface direction
				BlockFace({0, 0, 0}, {0, 1, 0}, {1, 1, 1}, {1, 0, 1}, {-1, 0, 0}),
				BlockFace({1, 0, 1}, {1, 1, 1}, {0, 1, 0}, {0, 0, 0}, {1, 0, 0}),
				BlockFace({1, 0, 0}, {1, 1, 0}, {0, 1, 1}, {0, 0, 1}, {0, 0, -1}),
				BlockFace({0, 0, 1}, {0, 1, 1}, {1, 1, 0}, {1, 0, 0}, {0, 0, 1}),
			}}
		},
		{// obscures directions (for face-to-face culling)
			{PX, false},
			{NX, false},
			{PY, false},
			{NY, false},
			{PZ, false},
			{NZ, false},
		},
		{
			AABB::FromMinMax({0.1f,0.0f,0.1f}, {0.9f,0.8f,0.9f})
		}
	}},

	{BLOCKSHAPE_SLAB,{
		{// map to list of faces for each direction
			{PX, {
				BlockFace({1, 0, 0}, {1, 0.5f, 0}, {1, 0.5f, 1}, {1, 0, 1}, {1, 0, 0})
			}},
			{NX, {
				BlockFace({0, 0, 1}, {0, 0.5f, 1}, {0, 0.5f, 0}, {0, 0, 0}, {-1, 0, 0})
			}},
			{PY, {
			}},
			{NY, {
				BlockFace({0, 0, 1}, {0, 0, 0}, {1, 0, 0}, {1, 0, 1}, {0, -1, 0})
			}},
			{PZ, {
				BlockFace({1, 0, 1}, {1, 0.5f, 1}, {0, 0.5f, 1}, {0, 0, 1}, {0, 0, 1})
			}},
			{NZ, {
				BlockFace({0, 0, 0}, {0, 0.5f, 0}, {1, 0.5f, 0}, {1, 0, 0}, {0, 0, -1})
			}},
			{UNDEFINED, { // undefined when the face doesnt meet a traditional surface direction
				BlockFace({0, 0.5f, 0}, {0, 0.5f, 1}, {1, 0.5f, 1}, {1, 0.5f, 0}, {0, 1, 0})
			}}
		},
		{// obscures directions (for face-to-face culling)
			{PX, false},
			{NX, false},
			{PY, false},
			{NY, true},
			{PZ, false},
			{NZ, false},
		},
		{
			AABB::FromMinMax({0,0,0}, {1,0.5f,1})
		}
	}},
};

BlockShape::BlockShape(map<BlockShapeDirection, vector<BlockFace>> faceMap, map<BlockShapeDirection, bool> obscures, vector<AABB> aabbs) : _faceMap(faceMap), _obscures(obscures), _aabbs(aabbs)
{
	Vector3 min, max;
	aabbs[0].GetMinMax(&min, &max);

	Vector3 curMin = min;
	Vector3 curMax = max;
	//for(const AABB& aabb : _aabbs) {
	//for(int i = 1; i < aabbs.size(); i++) {
	//	const AABB& aabb = aabbs[i];
	for(auto aabb = aabbs.begin()+1; aabb < aabbs.end(); aabb++) {
		aabb->GetMinMax(&min, &max);

		if(min.x < curMin.x) curMin.x = min.x;
		if(min.y < curMin.y) curMin.y = min.y;
		if(min.z < curMin.z) curMin.z = min.z;

		if(max.x > curMax.x) curMax.x = max.x;
		if(max.y > curMax.y) curMax.y = max.y;
		if(max.z > curMax.z) curMax.z = max.z;
	}

	maxAABB = AABB::FromMinMax(curMin, curMax);
}
