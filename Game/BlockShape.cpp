#include "BlockShape.h"

map<BlockShapeID, BlockShape> BlockShape::blockShapes = {
	{BLOCKSHAPE_BLOCK,{
		{
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
			{UNDEFINED, {
			}}
		}
	}}
};