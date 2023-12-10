#include "Crafting.h"

Vector2Int RecipeComponent::GetUVPos() const
{
	if(type == InventoryItem::Type::ITEM) {
		const Item& def = ItemDef::Get((ItemID)ID);
		return { def.UVx(), def.UVy() };
	}
	else {
		const Block& def = BlockDef::GetDef((BlockID)ID);
		return { def.GetTopUVidx(), def.GetTopUVidy() };
	}
}

vector<Category> Crafting::categories = {
	{"Blocks", {
		{"OAK_PLANKS", "SPRUCE_PLANKS", "BIRCH_PLANKS", "CHERRY_PLANKS"},
		{"STONE_COBBLE", "BLACKSTONE_COBBLE"},
	}},
	{"Items", {
		{"OAK_STICKS","CHERRY_STICKS","SPRUCE_STICKS","BIRCH_STICKS"},
	}},
	{"Food", {
		{},
	}},
	{"Misc", {
		{},
	}},
};

map<string, Recipe> Recipe::recipes = {
	{"STONE_COBBLE", {
		{STONE}, // Result (single RecipeComponent)
		{ // Input (vector of RecipeComponents)
			{COBBLESTONE,2}
		}
	}},
	{"BLACKSTONE_COBBLE",{
		{BLACKSTONE}, // Result (single RecipeComponent)
		{ // Input (vector)
			{BLACK_COBBLESTONE,2}
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
};