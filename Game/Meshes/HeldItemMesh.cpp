#include "HeldItemMesh.h"

Vertex heldItemVertices[] = {
		Vertex(0.0f, -.25f, .25f,	-1.0f, 0.0f, 0.0f,	0.0f, 1.0f), //bottom-left		12  -X
		Vertex(0.0f,  .25f, .25f,	-1.0f, 0.0f, 0.0f,	0.0f, 0.0f), //top-left			13
		Vertex(0.05f,  .25f, -.25f, -1.0f, 0.0f, 0.0f,	1.0f, 0.0f), //top-right		14
		Vertex(0.05f, -.25f, -.25f, -1.0f, 0.0f, 0.0f,	1.0f, 1.0f), //bottom-right		15
};

DWORD heldItemIndices[] = {
	0,1,2, //-z
	0,2,3,
};