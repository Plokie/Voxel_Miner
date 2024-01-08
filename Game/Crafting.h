#pragma once

#include <string>
#include <vector>
#include <map>

#include "Blocks.h"
#include "Items.h"
#include "InventoryItem.h"

using namespace std;

class RecipeComponent {
public:
	const InventoryItem::Type type = InventoryItem::Type::BLOCK;
	const unsigned short ID = 0;
	const int amount = 1;

	Vector2Int GetUVPos() const;

	/*operator tuple<int, int>() const {

	}*/

	/*operator XMVECTOR() const {
		return XMVectorSet(x, y, z, 0.0f);
	}*/

	RecipeComponent() {}
	RecipeComponent(BlockID blockID) : type(InventoryItem::Type::BLOCK), ID(blockID), amount(1) {}
	RecipeComponent(ItemID itemID) : type(InventoryItem::Type::ITEM), ID(itemID), amount(1) {}
	RecipeComponent(BlockID blockID, int amount) : type(InventoryItem::Type::BLOCK), ID(blockID), amount(amount) {}
	RecipeComponent(ItemID itemID, int amount) : type(InventoryItem::Type::ITEM), ID(itemID), amount(amount) {}
};


class Recipe {
private:
public:
	const RecipeComponent result;
	const vector<RecipeComponent> input;

	Recipe(){}
	Recipe(const RecipeComponent result, const vector<RecipeComponent> input) : result(result), input(input) {}

	
	static map<string, Recipe> recipes;
	//static const Recipe& Get(BlockID blockID) { return blocks.[blockID]; }
	//static const vector<const Recipe&> Get(BlockID blockID);
	//static const vector<const Recipe&> Get(ItemID itemID); 
	//static const Recipe& Get(ItemID itemID) { return items[itemID]; }
};

class Category {
public:
	const string name = "ERROR";
	const vector<vector<string>> recipes;
	
	Category() {}
	Category(const string name, const vector<vector<string>> recipes) : name(name), recipes(recipes) {}
};

class Crafting {
public:
	static vector<Category> workbenchCategories;
	//static vector<Category> furnaceCategories;

	// tuple of ID : Type
	static map<tuple<int, int>, RecipeComponent> furnaceRecipes;

	static bool GetFurnaceRecipe(unsigned short ID, InventoryItem::Type type, const RecipeComponent** outRecipeComponent);
	static bool GetFurnaceRecipe(BlockID blockID, const RecipeComponent** outRecipeComponent);
	static bool GetFurnaceRecipe(ItemID blockID, const RecipeComponent** outRecipeComponent);
	static bool GetFurnaceRecipe(InventoryItem* inputItem, const RecipeComponent** outRecipeComponent);


};