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
		}
	}}
};