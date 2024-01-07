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

class FurnaceUI : public TableInterface {
private:

public:
	void Open(InterfaceContext ctx) override;
	void Close() override;

	void Start() override;
	void Update(const float dt) override;
	void Draw(SpriteBatch* spriteBatch) override;
};