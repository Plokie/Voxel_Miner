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

	{COPPER_PICKAXE, Item("Copper Pickaxe", 1, 2, 1, PICKAXE)},
	{GOLD_PICKAXE, Item("Gold Pickaxe", 2, 2, 1, PICKAXE)},
	{AMETHYST_PICKAXE, Item("Amethyst Pickaxe", 3, 2, 1, PICKAXE)},
	{TITANIUM_PICKAXE, Item("Titanium Pickaxe", 4, 2, 1, PICKAXE)},

	{COPPER_AXE, Item("Copper Axe", 1, 3, 1, AXE)},
	{GOLD_AXE, Item("Gold Axe", 2, 3, 1, AXE)},
	{AMETHYST_AXE, Item("Amethyst Axe", 3, 3, 1, AXE)},
	{TITANIUM_AXE, Item("Titanium Axe", 4, 3, 1, AXE)},

	{COPPER_SHOVEL, Item("Copper Shovel", 1, 4, 1, SHOVEL)},
	{GOLD_SHOVEL, Item("Gold Shovel", 2, 4, 1, SHOVEL)},
	{AMETHYST_SHOVEL, Item("Amethyst Shovel", 3, 4, 1, SHOVEL)},
	{TITANIUM_SHOVEL, Item("Titanium Shovel", 4, 4, 1, SHOVEL)},

	{STICK, Item("Stick", 0, 2)},
};

const Item& ItemDef::Get(ItemID id)
{
	auto it = defs.find(id);
	if(it != defs.end()) {
		return it->second;
	}
	return Item("ERR", 0, 0);
}