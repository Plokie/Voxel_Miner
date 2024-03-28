#include "Crafting.h"

Vector2Int RecipeComponent::GetUVPos() const
{
	if(type == InventoryItem::Type::ITEM) {
		const Item& def = ItemDef::Get((ItemID)ID);
		return { def.UVx(), def.UVy() };
	}
	else {
		const Block& def = BlockDef::GetDef((BlockID)ID);
		return { def.GetSideUVidx(), def.GetSideUVidy() };
	}
}

vector<Category> Crafting::workbenchCategories = {
	//{"Workbench", {}},
	{"Utilities", {
		{"OAK_PLANKS", "SPRUCE_PLANKS", "BIRCH_PLANKS", "CHERRY_PLANKS"},
		{"FURNACE","FURNACE_BLACKSTONE"},
		{"WORKBENCH_OAK", "WORKBENCH_BIRCH", "WORKBENCH_SPRUCE", "WORKBENCH_CHERRY"},
		{"CHEST_OAK", "CHEST_BIRCH", "CHEST_SPRUCE", "CHEST_CHERRY"},
	}},
	{"Blocks", {
		{"OAK_PLANKS", "SPRUCE_PLANKS", "BIRCH_PLANKS", "CHERRY_PLANKS"},
		{"COAL_BLOCK", "COPPER_BLOCK", "GOLD_BLOCK", "AMETHYST_BLOCK"},
		{"TITANIUM_BLOCK", "COBBLESTONE_SLAB"},
		{"STONE_BRICKS","BLACKSTONE_BRICKS", "GRANITE_BRICKS"},
	}},
	{"Tools", {
		{"OAK_STICKS","CHERRY_STICKS","SPRUCE_STICKS","BIRCH_STICKS"},
		{"COPPER_PICKAXE","GOLD_PICKAXE","AMETHYST_PICKAXE","TITANIUM_PICKAXE"},
		{"COPPER_AXE","GOLD_AXE","AMETHYST_AXE","TITANIUM_AXE"},
		{"COPPER_SHOVEL","GOLD_SHOVEL","AMETHYST_SHOVEL","TITANIUM_SHOVEL"},
	}},
	//{"Minerals", {
	//	{"COPPER_BAR", "GOLD_BAR", "TITANIUM_BAR"},
	//}},
	{"Misc", {
		{"DEBUG_RECIPE_1","DEBUG_RECIPE_2","DEBUG_RECIPE_3","DEBUG_RECIPE_4"},
		{"REVERSE_COAL_BLOCK", "REVERSE_COPPER_BLOCK", "REVERSE_GOLD_BLOCK", "REVERSE_AMETHYST_BLOCK"},
		{"REVERSE_TITANIUM_BLOCK"},
	}},
};

//vector<Category> Crafting::furnaceCategories = {
//	//{"Furnace", {}},
//	//{"Blocks", {
//	//	{"OAK_PLANKS", "SPRUCE_PLANKS", "BIRCH_PLANKS", "CHERRY_PLANKS"},
//	//	{"STONE_COBBLE", "BLACKSTONE_COBBLE"},
//	//}},
//	//{"Tools", {
//	//	{"OAK_STICKS","CHERRY_STICKS","SPRUCE_STICKS","BIRCH_STICKS"},
//	//	{"COPPER_PICKAXE","GOLD_PICKAXE","AMETHYST_PICKAXE","TITANIUM_PICKAXE"},
//	//	{"COPPER_AXE","GOLD_AXE","AMETHYST_AXE","TITANIUM_AXE"},
//	//}},
//	{"Minerals", {
//		{"COPPER_BAR", "GOLD_BAR", "TITANIUM_BAR"},
//	}},
//	{"Food", {
//		/*{"COOKED_STEAK", "COOKED_CHICKEN"},
//		{"ROASTED_CARROT", "ROASTED_APPLE"},
//		{"FRIED_EGG"},*/
//	}},
//	//{"Misc", {
//	//	{"FURNACE","FURNACE_BLACKSTONE"},
//	//	{"WORKBENCH_OAK", "WORKBENCH_BIRCH", "WORKBENCH_SPRUCE", "WORKBENCH_CHERRY"},
//	//	{"DEBUG_RECIPE_1","DEBUG_RECIPE_2","DEBUG_RECIPE_3","DEBUG_RECIPE_4"}
//	//}},
//};

map<tuple<int, int>, RecipeComponent> Crafting::furnaceRecipes = {
	{{RAW_STEAK, InventoryItem::ITEM}, COOKED_STEAK},
	{{RAW_CHICKEN, InventoryItem::ITEM}, COOKED_CHICKEN},
	{{CARROT, InventoryItem::ITEM}, ROASTED_CARROT},
	{{EGG, InventoryItem::ITEM}, FRIED_EGG},
	{{APPLE, InventoryItem::ITEM}, ROASTED_APPLE},

	{{RAW_COPPER, InventoryItem::ITEM}, COPPER_BAR},
	{{RAW_GOLD, InventoryItem::ITEM}, GOLD_BAR},
	{{RAW_TITANIUM, InventoryItem::ITEM}, TITANIUM_BAR},

	{{SAND, InventoryItem::BLOCK}, GLASS},

	{{COBBLESTONE, InventoryItem::BLOCK}, STONE},
	{{BLACK_COBBLESTONE, InventoryItem::BLOCK}, BLACKSTONE},
	{{CLAY, InventoryItem::BLOCK}, BRICK_BLOCK},
};

map<string, Recipe> Recipe::recipes = {
	{"DEBUG_RECIPE_1", {
		{RAW_STEAK,1}, // Result (single RecipeComponent)
		{ // Input (vector of RecipeComponents)
			{COAL,3}
		}
	}},
	{"DEBUG_RECIPE_2", {
		{RAW_CHICKEN,1}, // Result (single RecipeComponent)
		{ // Input (vector of RecipeComponents)
			{COAL,2}
		}
	}},
	{"DEBUG_RECIPE_3", {
		{EGG,1}, // Result (single RecipeComponent)
		{ // Input (vector of RecipeComponents)
			{COAL,2}
		}
	}},
	{"DEBUG_RECIPE_4", {
		{CARROT,1}, // Result (single RecipeComponent)
		{ // Input (vector of RecipeComponents)
			{COAL,2}
		}
	}},

	{"STONE_COBBLE", {
		{STONE,5}, // Result (single RecipeComponent)
		{ // Input (vector of RecipeComponents)
			{COBBLESTONE,5},
			{COAL,1},
		}
	}},
	{"BLACKSTONE_COBBLE",{
		{BLACKSTONE,5}, // Result (single RecipeComponent)
		{ // Input (vector)
			{BLACK_COBBLESTONE,5},
			{COAL,1},
		}
	}},

	{"OAK_PLANKS",{ // Key (Just ID for lookup purposes)
		{OAK_PLANKS, 4}, // Result (single RecipeComponent)
		{ // Input (vector)
			{OAK_LOG,1}
		}
	}},
	{"SPRUCE_PLANKS",{ // Key (Just ID for lookup purposes)
		{SPRUCE_PLANKS, 4}, // Result (single RecipeComponent)
		{ // Input (vector)
			{SPRUCE_LOG,1}
		}
	} },
	{"BIRCH_PLANKS",{ // Key (Just ID for lookup purposes)
		{BIRCH_PLANKS, 4}, // Result (single RecipeComponent)
		{ // Input (vector)
			{BIRCH_LOG,1}
		}
	}},
	{"CHERRY_PLANKS",{ // Key (Just ID for lookup purposes)
		{CHERRY_PLANKS, 4}, // Result (single RecipeComponent)
		{ // Input (vector)
			{CHERRY_LOG,1}
		}
	}},

	{"OAK_STICKS",{ // Key (Just ID for lookup purposes)
		{STICK, 2}, // Result (single RecipeComponent)
		{ // Input (vector)
			{OAK_PLANKS,1}
		}
	}},
	{"CHERRY_STICKS",{ // Key (Just ID for lookup purposes)
		{STICK, 2}, // Result (single RecipeComponent)
		{ // Input (vector)
			{CHERRY_PLANKS,1}
		}
	}},
	{"SPRUCE_STICKS",{ // Key (Just ID for lookup purposes)
		{STICK, 2}, // Result (single RecipeComponent)
		{ // Input (vector)
			{SPRUCE_PLANKS,1}
		}
	}},
	{"BIRCH_STICKS",{ // Key (Just ID for lookup purposes)
		{STICK, 2}, // Result (single RecipeComponent)
		{ // Input (vector)
			{BIRCH_PLANKS,1}
		}
	}},

	{"COPPER_BAR",{
		{COPPER_BAR},
		{
			{RAW_COPPER, 5},
			{COAL, 2}
		}
	}},
	{"GOLD_BAR",{
		{GOLD_BAR},
		{
			{RAW_GOLD, 5},
			{COAL, 2}
		}
	}},
	{"TITANIUM_BAR",{
		{TITANIUM_BAR},
		{
			{RAW_TITANIUM, 5},
			{COAL, 2}
		}
	}},


	{"COPPER_PICKAXE",{
		{COPPER_PICKAXE}, 
		{ 
			{STICK,2},
			{COPPER_BAR, 3}
		}
	}},
	{"GOLD_PICKAXE",{
		{GOLD_PICKAXE}, 
		{ 
			{STICK,2},
			{GOLD_BAR, 3}
		}
	}},
	{"AMETHYST_PICKAXE",{
		{AMETHYST_PICKAXE}, 
		{ 
			{STICK,2},
			{AMETHYST, 10}
		}
	}},
	{"TITANIUM_PICKAXE",{
		{TITANIUM_PICKAXE}, 
		{ 
			{STICK,2},
			{TITANIUM_BAR, 3}
		}
	}},

	{"COPPER_AXE",{
		{COPPER_AXE},
		{
			{STICK,2},
			{COPPER_BAR, 3}
		}
	}},
	{"GOLD_AXE",{
		{GOLD_AXE},
		{
			{STICK,2},
			{GOLD_BAR, 3}
		}
	}},
	{"AMETHYST_AXE",{
		{AMETHYST_AXE},
		{
			{STICK,2},
			{AMETHYST, 10}
		}
	}},
	{"TITANIUM_AXE",{
		{TITANIUM_AXE},
		{
			{STICK,2},
			{TITANIUM_BAR, 3}
		}
	}},
	{"COPPER_SHOVEL",{
		{COPPER_SHOVEL},
		{
			{STICK,2},
			{COPPER_BAR, 2}
		}
	}},
	{"GOLD_SHOVEL",{
		{GOLD_SHOVEL},
		{
			{STICK,2},
			{GOLD_BAR, 2}
		}
	}},
	{"AMETHYST_SHOVEL",{
		{AMETHYST_SHOVEL},
		{
			{STICK,2},
			{AMETHYST, 6}
		}
	}},
	{"TITANIUM_SHOVEL",{
		{TITANIUM_SHOVEL},
		{
			{STICK,2},
			{TITANIUM_BAR, 2}
		}
	}},
	{ "FURNACE",{
		{FURNACE},
		{
			{COBBLESTONE,12}
		}
	}},
	{ "FURNACE_BLACKSTONE",{
		{FURNACE},
		{
			{BLACK_COBBLESTONE,12}
		}
	}},

	{ "WORKBENCH_OAK",{
		{WORKBENCH},
		{
			{OAK_PLANKS,8}
		}
	}},
	{ "WORKBENCH_BIRCH",{
		{WORKBENCH},
		{
			{BIRCH_PLANKS,8}
		}
	}},
	{ "WORKBENCH_SPRUCE",{
		{WORKBENCH},
		{
			{SPRUCE_PLANKS,8}
		}
	}},
	{ "WORKBENCH_CHERRY",{
		{WORKBENCH},
		{
			{CHERRY_PLANKS,8}
		}
	}},
		//
	{ "CHEST_OAK",{
		{CHEST},
		{
			{OAK_PLANKS,12}
		}
	} },
	{ "CHEST_BIRCH",{
		{CHEST},
		{
			{BIRCH_PLANKS,12}
		}
	} },
	{ "CHEST_SPRUCE",{
		{CHEST},
		{
			{SPRUCE_PLANKS,12}
		}
	} },
	{ "CHEST_CHERRY",{
		{CHEST},
		{
			{CHERRY_PLANKS,12}
		}
	} },

	{ "COAL_BLOCK",{
		{COAL_BLOCK},
		{
			{COAL,10}
		}
	} },
	{ "COPPER_BLOCK",{
		{COPPER_BLOCK},
		{
			{COPPER_BAR,10}
		}
	} },
	{ "GOLD_BLOCK",{
		{GOLD_BLOCK},
		{
			{GOLD_BAR,10}
		}
	} },
	{ "AMETHYST_BLOCK",{
		{AMETHYST_BLOCK},
		{
			{AMETHYST,10}
		}
	} },
	{ "TITANIUM_BLOCK",{
		{TITANIUM_BLOCK},
		{
			{TITANIUM_BAR,10}
		}
	}},

	{ "REVERSE_COAL_BLOCK",{
		{COAL, 10},
		{
			{COAL_BLOCK}
		}
	} },
	{ "REVERSE_COPPER_BLOCK",{
		{COPPER_BAR, 10},
		{
			{COPPER_BLOCK}
		}
	} },
	{ "REVERSE_GOLD_BLOCK",{
		{GOLD_BAR, 10},
		{
			{GOLD_BLOCK}
		}
	} },
	{ "REVERSE_AMETHYST_BLOCK",{
		{AMETHYST, 10},
		{
			{AMETHYST_BLOCK}
		}
	} },
	{ "REVERSE_TITANIUM_BLOCK",{
		{TITANIUM_BAR, 10},
		{
			{TITANIUM_BLOCK}
		}
	}},

	{ "STONE_BRICKS",{
		{STONE_BRICKS, 1},
		{
			{STONE,2}
		}
	}},
	{ "BLACKSTONE_BRICKS",{
		{BLACKSTONE_BRICKS, 1},
		{
			{BLACKSTONE,2}
		}
	} },
	{ "GRANITE_BRICKS",{
		{GRANITE_BRICKS, 1},
		{
			{GRANITE,2}
		}
	} },
	{ "COBBLESTONE_SLAB",{
		{COBBLESTONE_SLAB, 2},
		{
			{COBBLESTONE,1}
		}
	} },
};

bool Crafting::GetFurnaceRecipe(unsigned short ID, InventoryItem::Type type, const RecipeComponent** outRecipeComponent)
{
	auto findIt = furnaceRecipes.find({ ID, type });
	if(findIt != furnaceRecipes.end()) {
		*outRecipeComponent = &findIt->second;

		return true;
	}

	/*for(const auto& kvp : furnaceRecipes) {
		if(get<0>(kvp.first) == ID && get<1>(kvp.first) == type) {
			*outRecipeComponent = &kvp.second;

			return true;
		}
	}*/
	return false;
}

bool Crafting::GetFurnaceRecipe(BlockID blockID, const RecipeComponent** outRecipeComponent)
{
	return GetFurnaceRecipe(blockID, InventoryItem::BLOCK, outRecipeComponent);
}

bool Crafting::GetFurnaceRecipe(ItemID itemID, const RecipeComponent** outRecipeComponent)
{
	return GetFurnaceRecipe(itemID, InventoryItem::BLOCK, outRecipeComponent);
}

bool Crafting::GetFurnaceRecipe(InventoryItem* invItem, const RecipeComponent** outRecipeComponent)
{
	return GetFurnaceRecipe(invItem->ID, invItem->type, outRecipeComponent);
}

