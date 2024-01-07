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

class CraftingUI : public TableInterface {
private:
	Inventory* playerInv = nullptr;

	vector<Button*> _spawnedTabs;
	vector<Button*> _spawnedButtons;
	vector<ItemIcon*> _spawnedRecipeIcons;

	UIRect* recipeDisplayBg = nullptr;
	Button* craftButton = nullptr;

	void DeleteCategoryObjects();
	void DeleteRecipeObjects();

	void LoadCategory(const Category& category);
	void SelectRecipe(const Recipe& recipe);
public:
	void Open(InterfaceContext ctx) override;
	void Close() override;

	void Start() override;
	void Update(const float dt) override;
	void Draw(SpriteBatch* spriteBatch) override;
};

//class CraftingUI : public UIRect {
//private:
//	Inventory* inventory = nullptr;
//
//	//HorizontalLayoutRect tabs;
//	//map<Button*, Category*> _spawnedTabs;
//	vector<Button*> _spawnedTabs;
//	
//	vector<Button*> _spawnedButtons;
//	vector<ItemIcon*> _spawnedItemIcons;
//
//	vector<ItemIcon*> _spawnedRecipeIcons;
//
//	UIRect* recipeDisplayBg = nullptr;
//	Button* craftButton = nullptr;
//
//	void DeleteCategoryObjects();
//	void DeleteRecipeObjects();
//
//public:
//	CraftingUI();
//	~CraftingUI();
//
//	void Create();
//
//	void LoadCategory(const Category* category);
//	void SelectRecipe(const Recipe& recipe);
//
//	void Update(const float dt) override;
//	void Draw(SpriteBatch* spriteBatch) override;
//};