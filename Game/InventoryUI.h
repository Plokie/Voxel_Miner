#pragma once

#include <vector>
#include "../Engine/Object2D.h"

class Engine;
class Scene;
class HorizontalLayoutRect;
class UIRect;

#define INVSIZE_X 9
#define INVSIZE_Y 4 // Includes hotbar

class InventoryUI : public Object2D {
private:
	HorizontalLayoutRect* hotbar = nullptr;
	UIRect* invBg = nullptr;

	// pair of slot parent and slot icon
	UIRect* invSlots[INVSIZE_X][INVSIZE_Y];
	vector<UIRect*> _spawnedIcons = {};

	bool isOpen = false;

public:
	InventoryUI(Engine* engine, Scene* gameScene);

	void Open();
	void Close();
	void Update(const float dTime) override;
	void Start() override;
};