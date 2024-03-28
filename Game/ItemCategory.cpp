#include "ItemCategory.h"

map<ITEM_CATEGORY, string> ItemCategory::categoryName = {
	{CAT_NONE, "Err:None"},
	{CAT_NATURE, "Nature"},
	{CAT_BUILDING, "Building"},
	{CAT_UTILITY, "Utility"},
	//{CAT_TOOLS, "Tools"},
	{CAT_FOOD, "Food"},
	{CAT_MISC, "Misc"},
};
