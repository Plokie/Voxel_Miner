#pragma once

#include "../Engine/Object2D.h"
#include "InventoryItem.h"

class UIRect;
class Label;
class InventoryUI;
class RecipeComponent;

class ItemIcon : public Object2D {
private:
	UIRect* icon = nullptr;
	Label* amtLabel = nullptr;
	Label* nameLabel = nullptr;
	InventoryUI* invUI = nullptr;
	InventoryItem* invItem = nullptr;

	bool isHovering = false;
public:
	bool isHeld = false;
	InventoryItem* GetInvItem() const { return invItem; }

	ItemIcon(InventoryItem* invItem, InventoryUI* invUI);
	ItemIcon(const RecipeComponent& recipeComponent);
	~ItemIcon();
	
	void Display(const unsigned short ID, const InventoryItem::Type type, const int amount, const Vector2Int& uvPos, const bool hideOne = true);

	const bool WasPressed();
	const bool WasRightClicked();

	const Vector2 GetScreenPosition() override;
	void Draw(SpriteBatch* spriteBatch) override;
	void SetDimensions(const Vector2& dim) override;
	void Update(const float dTime) override;
};