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
class Label;
class ProgressBar;

class FurnaceUI : public TableInterface {
private:
	ProgressBar* fuelProgress;
	ProgressBar* cookProgress;


public:
	void Open(InterfaceContext ctx) override;
	void Close() override;

	void Start() override;
	void Update(const float dt) override;
	void Draw(SpriteBatch* spriteBatch) override;
};