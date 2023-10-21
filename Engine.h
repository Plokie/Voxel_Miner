#pragma once

#include "Graphics.h"
#include "WinManager.h"

class Engine {
private:
	Graphics gfx;
	WinManager winMgr;
public:
	void Init(_In_ HINSTANCE hInstance);

	void Render();

	/// <summary>
	/// Call at start of main while loop loop
	/// </summary>
	/// <returns>bool state, if to continue rendering</returns>
	bool Service();
};