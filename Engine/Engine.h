#pragma once

#include "../Graphics/Graphics.h"
#include "WinManager.h"
#include "Input.h"

class Engine {
private:
	Graphics gfx;
	WinManager winMgr;
public:
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