#pragma once

#include <vector>
#include <map>

#include "../Graphics/Graphics.h"
#include "WinManager.h"
#include "Input.h"
#include "Transform.h"
//#include "World.h";
#include "Object3D.h"
#include "..\Graphics\Resources.h"

using namespace std;

class Engine {
private:
	Graphics gfx;
	WinManager winMgr;
	//World world;
public:
	map<string, Object3D*> sceneObjects = {};
	
	// Initialise Engine
	void Init(_In_ HINSTANCE hInstance);

	void Render(float dTime);
	void Update(float dTime);

	/// <summary>
	/// Call at start of main while loop loop
	/// </summary>
	/// <returns>if to continue running</returns>
	bool Service();

	~Engine();
};