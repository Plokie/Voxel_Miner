#pragma once

#include <map>
#include "../Engine/UI/UIRect.h"
#include "../Engine/UI/Button.h"
#include "TableInterface.h"

class Category;
class ItemIcon;
class Recipe;
class Inventory;
class Button;

#define CHESTSIZE_X 9
#define CHESTSIZE_Y 3

class CreativeUI : public TableInterface {
private:

	void ClearList();
	void DrawList(vector<pair<unsigned short, InventoryItem::Type>>& vec);
public:
	void Open(InterfaceContext ctx) override;
	void Close() override;

	void LoadSearch(const string& search);
	void LoadCategory(ITEM_CATEGORY itemCategory);

	void Start() override;
	void Update(const float dt) override;
	void Draw(SpriteBatch* spriteBatch) override;
};