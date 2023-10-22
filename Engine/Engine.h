#pragma once

#include "../Graphics/Graphics.h"
#include "WinManager.h"
#include "Input.h"
#include "Transform.h"
//#include "World.h";

class Engine {
private:
	Graphics gfx;
	WinManager winMgr;
	//World world;
public:
	
	/*Engine() {
		world = World();
	}*/

	// Initialise Engine
	void Init(_In_ HINSTANCE hInstance);

	void Render(float dTime);
	void Update(float dTime);

	/// <summary>
	/// Call at start of main while loop loop
	/// </summary>
	/// <returns>if to continue running</returns>
	bool Service();
};