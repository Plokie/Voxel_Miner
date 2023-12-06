#pragma once

#include "../Engine/Object2D.h"

class UIRect;
class Label;
struct InventoryItem;

class ItemIcon : public Object2D {
private:
	UIRect* icon = nullptr;
	Label* amtLabel = nullptr;

public:

	ItemIcon(const InventoryItem* invItem);
	~ItemIcon();
	
	void Draw(SpriteBatch* spriteBatch) override;
	void SetDimensions(const Vector2& dim) override;
};