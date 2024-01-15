#pragma once

#include <vector>
#include "../Engine/Object2D.h"
#include "Inventory.h"
#include "TableInterface.h"

class Engine;
class Scene;
class HorizontalLayoutRect;
class UIRect;
class Inventory;
class ItemIcon;
class Button;
class PlayerData;

class InventoryUI : public Object2D {
private:
	HorizontalLayoutRect* hotbar = nullptr;
	UIRect* invBg = nullptr;
	
	PlayerData* _pPlayerData = nullptr;
	//Inventory* inventory = nullptr;

	// pair of slot parent and slot icon
	Button* invSlots[INVSIZE_X][INVSIZE_Y];
	Button* hotbarSlots[INVSIZE_X];
	vector<ItemIcon*> _spawnedIcons = {};
	vector<ItemIcon*> _hotbarIcons = {};
	UIRect* hotbarSelect = nullptr;

	bool isOpen = false;

	TableInterface* currentInterface = nullptr;

	//void AttemptMoveFromStorage(ItemIcon* itemIcon);

	// held item by the mouse when in inventory ui
public:
	ItemIcon* heldItem = nullptr;
	InventoryUI(Engine* engine, Scene* gameScene);
	~InventoryUI();

	Inventory* GetInventory() const;
	Button* GetInvSlot(int x, int y) const { return invSlots[x][y]; }
	
	void AddNewIcon(ItemIcon* itemIcon);
	void DeleteIcon(ItemIcon* itemIcon);

	bool canClose = true;

	//void SetHeldItem(ItemIcon* invItem);
	//void SlotPressed(Button* slot);

	void ReleaseItem(ItemIcon* invItem);

	void FlagItemIconCooldown(InventoryItem* invItemChild);

	void Open();
	void Open(TableInterface* tableInterface, InterfaceContext ctx);
	void Close();
	void ReloadIcons();
	void HardReloadIcons();
	void TableReload(Vector3Int tablePosition);
	void Update(const float dTime) override;
	void Start() override;
	void Draw(SpriteBatch* spriteBatch) override;

	bool EraseIcon(ItemIcon* icon);

	void DrawHotbarIcons();
};