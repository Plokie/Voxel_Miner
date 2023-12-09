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
class Button;

class InventoryUI : public Object2D {
private:
	HorizontalLayoutRect* hotbar = nullptr;
	UIRect* invBg = nullptr;
	Inventory* inventory = nullptr;

	// pair of slot parent and slot icon
	Button* invSlots[INVSIZE_X][INVSIZE_Y];
	Button* hotbarSlots[INVSIZE_X];
	vector<ItemIcon*> _spawnedIcons = {};
	vector<ItemIcon*> _hotbarIcons = {};
	UIRect* hotbarSelect = nullptr;

	bool isOpen = false;

	// held item by the mouse when in inventory ui
	ItemIcon* heldItem = nullptr;
public:
	InventoryUI(Engine* engine, Scene* gameScene);
	~InventoryUI();

	Inventory* GetInventory() const { return inventory; }
	Button* GetInvSlot(int x, int y) const { return invSlots[x][y]; }
	

	//void SetHeldItem(ItemIcon* invItem);
	//void SlotPressed(Button* slot);

	void ReleaseItem(ItemIcon* invItem);

	void Open();
	void Close();
	void Update(const float dTime) override;
	void Start() override;
	void Draw(SpriteBatch* spriteBatch) override;

	void DrawHotbarIcons();
};