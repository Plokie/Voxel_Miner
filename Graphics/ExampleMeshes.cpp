#include "ExampleMeshes.h"

Vertex exampleCubeVertices[] = {
		Vertex(-1.0f, -1.0f, -1.0f,	0.0f, 0.0f, -1.0f,	0.0f, 1.0f), //bottom-left		0  -Z
		Vertex(-1.0f,  1.0f, -1.0f,	0.0f, 0.0f, -1.0f,	0.0f, 0.0f), //top-left			1
		Vertex(1.0f,  1.0f, -1.0f,	0.0f, 0.0f, -1.0f,	1.0f, 0.0f), //top-right		2
		Vertex(1.0f, -1.0f, -1.0f,	0.0f, 0.0f, -1.0f,	1.0f, 1.0f), //bottom-right		3

		Vertex(1.0f, -1.0f, 1.0f,	0.0f, 0.0f, 1.0f,	0.0f, 1.0f), //bottom-left		4  +Z
		Vertex(1.0f,  1.0f, 1.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f), //top-left			5
		Vertex(-1.0f,  1.0f, 1.0f,	0.0f, 0.0f, 1.0f,	1.0f, 0.0f), //top-right			6
		Vertex(-1.0f, -1.0f, 1.0f,	0.0f, 0.0f, 1.0f,	1.0f, 1.0f), //bottom-right		7

		Vertex(1.0f, -1.0f, -1.0f,	1.0f, 0.0f, 0.0f,	0.0f, 1.0f), //bottom-left		8  +X
		Vertex(1.0f,  1.0f, -1.0f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f), //top-left			9
		Vertex(1.0f,  1.0f, 1.0f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f), //top-right		10
		Vertex(1.0f, -1.0f, 1.0f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f), //bottom-right		11

		Vertex(-1.0f, -1.0f, 1.0f,	-1.0f, 0.0f, 0.0f,	0.0f, 1.0f), //bottom-left		12  -X
		Vertex(-1.0f,  1.0f, 1.0f,	-1.0f, 0.0f, 0.0f,	0.0f, 0.0f), //top-left			13
		Vertex(-1.0f,  1.0f, -1.0f, -1.0f, 0.0f, 0.0f,	1.0f, 0.0f), //top-right		14
		Vertex(-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,	1.0f, 1.0f), //bottom-right		15

		Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f,	0.0f, 1.0f), //					16 +Y
		Vertex(-1.0f,  1.0f, 1.0f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f), //					17
		Vertex(1.0f,  1.0f, 1.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f), //					18
		Vertex(1.0f,  1.0f, -1.0f,	0.0f, 1.0f, 0.0f,	1.0f, 1.0f), //					19

		Vertex(-1.0f, -1.0f, 1.0f,	0.0f, -1.0f, 0.0f,	0.0f, 1.0f), //					20 -Y
		Vertex(-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f,	0.0f, 0.0f), //					21
		Vertex(1.0f, -1.0f, -1.0f,  0.0f, -1.0f, 0.0f,	1.0f, 0.0f), //					22
		Vertex(1.0f, -1.0f, 1.0f,	0.0f, -1.0f, 0.0f,	1.0f, 1.0f), //					23
};

Vertex chunkBorderVertices[] = {
		Vertex(0.0f,  0.0f, 0.0f,	0.0f, 0.0f, -1.0f,	0.0f, 1.0f), //bottom-left		0  -Z
		Vertex(0.0f,  16.0f, 0.0f,	0.0f, 0.0f, -1.0f,	0.0f, 0.0f), //top-left			1
		Vertex(16.0f,  16.0f, 0.0f,	0.0f, 0.0f, -1.0f,	1.0f, 0.0f), //top-right		2
		Vertex(16.0f,  0.0f, 0.0f,	0.0f, 0.0f, -1.0f,	1.0f, 1.0f), //bottom-right		3

		Vertex(16.0f,  0.0f, 16.0f,	0.0f, 0.0f, 1.0f,	0.0f, 1.0f), //bottom-left		4  +Z
		Vertex(16.0f,  16.0f, 16.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f), //top-left			5
		Vertex(0.0f,  16.0f, 16.0f,	0.0f, 0.0f, 1.0f,	1.0f, 0.0f), //top-right			6
		Vertex(0.0f,  0.0f, 16.0f,	0.0f, 0.0f, 1.0f,	1.0f, 1.0f), //bottom-right		7

		Vertex(16.0f,  0.0f, 0.0f,	1.0f, 0.0f, 0.0f,	0.0f, 1.0f), //bottom-left		8  +X
		Vertex(16.0f,  16.0f, 0.0f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f), //top-left			9
		Vertex(16.0f,  16.0f, 16.0f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f), //top-right		10
		Vertex(16.0f,  0.0f, 16.0f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f), //bottom-right		11

		Vertex(0.0f,  0.0f, 16.0f,	-1.0f, 0.0f, 0.0f,	0.0f, 1.0f), //bottom-left		12  -X
		Vertex(0.0f,  16.0f, 16.0f,	-1.0f, 0.0f, 0.0f,	0.0f, 0.0f), //top-left			13
		Vertex(0.0f,  16.0f, 0.0f,	-1.0f, 0.0f, 0.0f,	1.0f, 0.0f), //top-right		14
		Vertex(0.0f,  0.0f, 0.0f,	-1.0f, 0.0f, 0.0f,	1.0f, 1.0f), //bottom-right		15

		Vertex(0.0f,  16.0f, 0.0f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f), //					16 +Y
		Vertex(0.0f,  16.0f, 16.0f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f), //					17
		Vertex(16.0f,  16.0f, 16.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f), //					18
		Vertex(16.0f,  16.0f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 1.0f), //					19

		Vertex(0.0f,  0.0f, 16.0f,	0.0f, -1.0f, 0.0f,	0.0f, 1.0f), //					20 -Y
		Vertex(0.0f,  0.0f, 0.0f,   0.0f, -1.0f, 0.0f,	0.0f, 0.0f), //					21
		Vertex(16.0f,  0.0f, 0.0f,   0.0f, -1.0f, 0.0f,	1.0f, 0.0f), //					22
		Vertex(16.0f,  0.0f, 16.0f,	0.0f, -1.0f, 0.0f,	1.0f, 1.0f), //					23
};


DWORD exampleCubeIndices[] = {
	0,1,2, //-Z
	0,2,3,

	4,5,6, //+Z
	4,6,7,

	8,9,10, //+X
	8,10,11,

	12,13,14, //-X
	12,14,15,

	16,17,18, //+Y
	16,18,19,

	20,21,22, //-Y
	20,22,23
};



Vertex exampleFloorVertices[] = {
		Vertex(-1.0f, -1.0f, -1.0f,	0.0f, 0.0f, -1.0f,	0.0f, 1.0f), //bottom-left		0  -Z
		Vertex(-1.0f,  1.0f, -1.0f,	0.0f, 0.0f, -1.0f,	0.0f, 0.0f), //top-left			1
		Vertex(1.0f,  1.0f, -1.0f,	0.0f, 0.0f, -1.0f,	1.0f, 0.0f), //top-right		2
		Vertex(1.0f, -1.0f, -1.0f,	0.0f, 0.0f, -1.0f,	1.0f, 1.0f), //bottom-right		3

		Vertex(1.0f, -1.0f, 1.0f,	0.0f, 0.0f, 1.0f,	0.0f, 1.0f), //bottom-left		4  +Z
		Vertex(1.0f,  1.0f, 1.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f), //top-left			5
		Vertex(-1.0f,  1.0f, 1.0f,	0.0f, 0.0f, 1.0f,	1.0f, 0.0f), //top-right			6
		Vertex(-1.0f, -1.0f, 1.0f,	0.0f, 0.0f, 1.0f,	1.0f, 1.0f), //bottom-right		7

		Vertex(1.0f, -1.0f, -1.0f,	1.0f, 0.0f, 0.0f,	0.0f, 1.0f), //bottom-left		8  +X
		Vertex(1.0f,  1.0f, -1.0f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f), //top-left			9
		Vertex(1.0f,  1.0f, 1.0f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f), //top-right		10
		Vertex(1.0f, -1.0f, 1.0f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f), //bottom-right		11

		Vertex(-1.0f, -1.0f, 1.0f,	-1.0f, 0.0f, 0.0f,	0.0f, 1.0f), //bottom-left		12  -X
		Vertex(-1.0f,  1.0f, 1.0f,	-1.0f, 0.0f, 0.0f,	0.0f, 0.0f), //top-left			13
		Vertex(-1.0f,  1.0f, -1.0f, -1.0f, 0.0f, 0.0f,	1.0f, 0.0f), //top-right		14
		Vertex(-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,	1.0f, 1.0f), //bottom-right		15

		Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f,	0.0f, 100.0f), //					16 +Y
		Vertex(-1.0f,  1.0f, 1.0f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f), //					17
		Vertex(1.0f,  1.0f, 1.0f,	0.0f, 1.0f, 0.0f,	100.0f, 0.0f), //					18
		Vertex(1.0f,  1.0f, -1.0f,	0.0f, 1.0f, 0.0f,	100.0f, 100.0f), //					19

		Vertex(-1.0f, -1.0f, 1.0f,	0.0f, -1.0f, 0.0f,	0.0f, 100.0f), //					20 -Y
		Vertex(-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f,	0.0f, 0.0f), //					21
		Vertex(1.0f, -1.0f, -1.0f,  0.0f, -1.0f, 0.0f,	100.0f, 0.0f), //					22
		Vertex(1.0f, -1.0f, 1.0f,	0.0f, -1.0f, 0.0f,	100.0f, 100.0f), //					23
};