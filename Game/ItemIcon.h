#pragma once

#include "../Engine/Object2D.h"
#include "InventoryItem.h"

class UIRect;
class Label;
class InventoryUI;
class RecipeComponent;
class Button;
class TableInterface;
class Inventory;

class ItemIcon : public Object2D {
private:
	UIRect* icon = nullptr;
	Label* amtLabel = nullptr;
	Label* nameLabel = nullptr;
	InventoryUI* invUI = nullptr;
	InventoryItem* invItem = nullptr;
	Inventory* inv = nullptr;

	bool isHovering = false;
	
	void AttemptStorageMigration(Inventory* targetInventory, TableInterface* tableInterface);
public:
	bool onPlacedownCooldown = false; // Used to ignore pickups for a frame
	bool isHeld = false;
	InventoryItem* GetInvItem() const { return invItem; }

	ItemIcon(InventoryItem* invItem, InventoryUI* invUI);
	ItemIcon(const RecipeComponent& recipeComponent);
	ItemIcon(BlockID blockID);
	ItemIcon(ItemID itemID);
	ItemIcon(unsigned short ID, InventoryItem::Type type);
	~ItemIcon();
	
	void Display(const unsigned short ID, const InventoryItem::Type type, const int amount, const Vector2Int& uvPos, const bool hideOne = true);

	const bool WasPressed();
	const bool WasRightClicked();
	void Reload();

	void SetInventoryParent(Inventory* inv);
	Inventory* GetInventoryParent() { return inv; }

	void ReleaseFunction(int idx, int idy, Button* parentSlot, Inventory* selfInventory, TableInterface* tableInterface = nullptr);

	const Vector2 GetScreenPosition() override;
	void Draw(SpriteBatch* spriteBatch) override;
	void SetDimensions(const Vector2& dim) override;
	void Update(const float dTime) override;
};