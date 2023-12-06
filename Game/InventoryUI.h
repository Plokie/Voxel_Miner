#pragma once

#include <vector>
#include "../Engine/Object2D.h"
#include "Inventory.h"

class Engine;
class Scene;
class HorizontalLayoutRect;
class UIRect;
class Inventory;
class ItemIcon;

class InventoryUI : public Object2D {
private:
	HorizontalLayoutRect* hotbar = nullptr;
	UIRect* invBg = nullptr;
	Inventory* inventory = nullptr;

	// pair of slot parent and slot icon
	UIRect* invSlots[INVSIZE_X][INVSIZE_Y];
	UIRect* hotbarSlots[INVSIZE_X];
	vector<ItemIcon*> _spawnedIcons = {};
	vector<ItemIcon*> _hotbarIcons = {};

	bool isOpen = false;

public:
	InventoryUI(Engine* engine, Scene* gameScene);
	~InventoryUI();

	void Open();
	void Close();
	void Update(const float dTime) override;
	void Start() override;
	void Draw(SpriteBatch* spriteBatch) override;

	void DrawHotbarIcons();
};