#include "Items.h"

#include "Inventory.h"

map<ItemID, BlockAction> Item::itemActions = {
	
};



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

	{COPPER_PICKAXE, Item("Copper Pickaxe", 1, 2, 1, PICKAXE, 1)},
	{GOLD_PICKAXE, Item("Gold Pickaxe", 2, 2, 1, PICKAXE, 2)},
	{AMETHYST_PICKAXE, Item("Amethyst Pickaxe", 3, 2, 1, PICKAXE, 3)},
	{TITANIUM_PICKAXE, Item("Titanium Pickaxe", 4, 2, 1, PICKAXE, 4)},

	{COPPER_AXE, Item("Copper Axe", 1, 3, 1, AXE, 1)},
	{GOLD_AXE, Item("Gold Axe", 2, 3, 1, AXE, 2)},
	{AMETHYST_AXE, Item("Amethyst Axe", 3, 3, 1, AXE, 3)},
	{TITANIUM_AXE, Item("Titanium Axe", 4, 3, 1, AXE, 4)},

	{COPPER_SHOVEL, Item("Copper Shovel", 1, 4, 1, SHOVEL, 1)},
	{GOLD_SHOVEL, Item("Gold Shovel", 2, 4, 1, SHOVEL, 2)},
	{AMETHYST_SHOVEL, Item("Amethyst Shovel", 3, 4, 1, SHOVEL, 3)},
	{TITANIUM_SHOVEL, Item("Titanium Shovel", 4, 4, 1, SHOVEL, 4)},

	{STICK, Item("Stick", 0, 2)},

	{RAW_STEAK, Item("Raw Steak", 5, 0, MAX_STACK, FOOD, 3)},
	{COOKED_STEAK, Item("Cooked Steak", 5, 1, MAX_STACK, FOOD, 6)},

	{RAW_CHICKEN, Item("Raw Chicken", 6, 0, MAX_STACK, FOOD, 2)},
	{COOKED_CHICKEN, Item("Cooked Chicken", 6, 1, MAX_STACK, FOOD, 7)},

	{CARROT, Item("Carrot", 7, 0, MAX_STACK, FOOD, 2)},
	{ROASTED_CARROT, Item("Roasted Carrot", 7, 1, MAX_STACK, FOOD, 4)},

	{EGG, Item("Egg", 8, 0)},
	{FRIED_EGG, Item("Fried Egg", 8, 1, MAX_STACK, FOOD, 4)},

	{APPLE, Item("Apple", 9, 0, MAX_STACK, FOOD, 2)},
	{ROASTED_APPLE, Item("Roasted Apple", 9, 1, MAX_STACK, FOOD, 4)},
};

const Item& ItemDef::Get(ItemID id)
{
	auto it = defs.find(id);
	if(it != defs.end()) {
		return it->second;
	}
	return defs.at(ITEMERR);
}

bool Item::CallItemAction(ItemID itemID, PlayerController* playerController, Inventory* inv, ChunkManager* chunkManager, Vector3Int targetBlockPos) {
	const Item& def = ItemDef::Get(itemID);
	if(def.itemType == FOOD && (inv->GetHunger() < HUNGER_MAX || (inv->GetHealth() < HEALTH_MAX && inv->GetSaturation()<=0 ))) {
		inv->ChangeHunger(def.tier);
		inv->SubHeldItem(); // this could cause logic errors when using an item that isnt held. But i dont see that happening
	}

	auto it = itemActions.find(itemID);
	if(it != itemActions.end()) {
		it->second.Invoke({ playerController, inv, chunkManager, targetBlockPos });
		return true;
	}
	return false;
}
