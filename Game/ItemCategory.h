#pragma once

#include <map>
#include <string>

using namespace std;

enum ITEM_CATEGORY : short {
	CAT_NONE = -1,
	CAT_NATURE,
	CAT_BUILDING,
	CAT_UTILITY,
	CAT_FOOD,
	CAT_MISC,

	CAT_COUNT
};

class ItemCategory {
public:
	static map<ITEM_CATEGORY, string> categoryName;
};