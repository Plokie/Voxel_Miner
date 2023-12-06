#include "Items.h"







//

const map<ItemID, Item> ItemDef::defs = {
	{ITEMERR, Item("ERR", 0, 0)},

	{COAL, Item("Coal", 0, 0)},
	{RAW_COPPER, Item("Raw Copper", 1, 0)},
	{RAW_GOLD, Item("Raw Gold", 2, 0)},
	{AMETHYST, Item("Amethyst", 3, 0)},
	{RAW_TITANIUM, Item("Raw Titanium", 4, 0)},

	{COPPER_BAR, Item("Copper Bar", 1, 1)},
	{GOLD_BAR, Item("Gold Bar", 2, 1)},
	{TITANIUM_BAR, Item("Titanium Bar", 4, 1)},

	{COPPER_PICKAXE, Item("Copper Pickaxe", 1, 2, 1)},
	{GOLD_PICKAXE, Item("Gold Pickaxe", 2, 2, 1)},
	{AMETHYST_PICKAXE, Item("Amethyst Pickaxe", 3, 2, 1)},
	{TITANIUM_PICKAXE, Item("Titanium Pickaxe", 4, 2, 1)},

	{COPPER_AXE, Item("Copper Axe", 1, 2, 1)},
	{GOLD_AXE, Item("Gold Axe", 2, 2, 1)},
	{AMETHYST_AXE, Item("Amethyst Axe", 3, 2, 1)},
	{TITANIUM_AXE, Item("Titanium Axe", 4, 2, 1)},
};

const Item& ItemDef::Get(ItemID id)
{
	auto it = defs.find(id);
	if(it != defs.end()) {
		return it->second;
	}
	return Item("ERR", 0, 0);
}