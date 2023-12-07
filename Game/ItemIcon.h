#pragma once

#include "../Engine/Object2D.h"

class UIRect;
class Label;
struct InventoryItem;
class InventoryUI;

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
	~ItemIcon();
	
	const bool WasPressed();

	const Vector2& GetScreenPosition() override;
	void Draw(SpriteBatch* spriteBatch) override;
	void SetDimensions(const Vector2& dim) override;
	void Update(const float dTime) override;
};