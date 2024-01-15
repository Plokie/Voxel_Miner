#include "Items.h"

#include "Inventory.h"
#include "../Audio/Audio.h"
#include "PlayerData.h"

map<ItemID, BlockAction> Item::itemActions = {
	
};



//

const map<ItemID, Item> ItemDef::defs = {
	{ITEMERR, Item("ERR", CAT_NONE, 0, 0)},

	{COAL, Item("Coal", CAT_MISC, 0, 0)},
	{RAW_COPPER, Item("Raw Copper", CAT_MISC, 1, 0)},
	{RAW_GOLD, Item("Raw Gold", CAT_MISC, 2, 0)},
	{AMETHYST, Item("Amethyst", CAT_MISC, 3, 0)},
	{RAW_TITANIUM, Item("Raw Titanium", CAT_MISC, 4, 0)},

	{COPPER_BAR, Item("Copper Bar", CAT_MISC, 1, 1)},
	{GOLD_BAR, Item("Gold Bar", CAT_MISC, 2, 1)},
	{TITANIUM_BAR, Item("Titanium Bar", CAT_MISC, 4, 1)},

	{COPPER_PICKAXE, Item("Copper Pickaxe", CAT_TOOLS, 1, 2, 1, PICKAXE, 1)},
	{GOLD_PICKAXE, Item("Gold Pickaxe", CAT_TOOLS, 2, 2, 1, PICKAXE, 2)},
	{AMETHYST_PICKAXE, Item("Amethyst Pickaxe", CAT_TOOLS, 3, 2, 1, PICKAXE, 3)},
	{TITANIUM_PICKAXE, Item("Titanium Pickaxe", CAT_TOOLS, 4, 2, 1, PICKAXE, 4)},

	{COPPER_AXE, Item("Copper Axe", CAT_TOOLS, 1, 3, 1, AXE, 1)},
	{GOLD_AXE, Item("Gold Axe", CAT_TOOLS, 2, 3, 1, AXE, 2)},
	{AMETHYST_AXE, Item("Amethyst Axe", CAT_TOOLS, 3, 3, 1, AXE, 3)},
	{TITANIUM_AXE, Item("Titanium Axe", CAT_TOOLS, 4, 3, 1, AXE, 4)},

	{COPPER_SHOVEL, Item("Copper Shovel", CAT_TOOLS, 1, 4, 1, SHOVEL, 1)},
	{GOLD_SHOVEL, Item("Gold Shovel", CAT_TOOLS, 2, 4, 1, SHOVEL, 2)},
	{AMETHYST_SHOVEL, Item("Amethyst Shovel", CAT_TOOLS, 3, 4, 1, SHOVEL, 3)},
	{TITANIUM_SHOVEL, Item("Titanium Shovel", CAT_TOOLS, 4, 4, 1, SHOVEL, 4)},

	{STICK, Item("Stick", CAT_MISC, 0, 2)},

	{RAW_STEAK, Item("Raw Steak", CAT_FOOD, 5, 0, MAX_STACK, FOOD, 3)},
	{COOKED_STEAK, Item("Cooked Steak", CAT_FOOD, 5, 1, MAX_STACK, FOOD, 6)},

	{RAW_CHICKEN, Item("Raw Chicken", CAT_FOOD, 6, 0, MAX_STACK, FOOD, 2)},
	{COOKED_CHICKEN, Item("Cooked Chicken", CAT_FOOD, 6, 1, MAX_STACK, FOOD, 7)},

	{CARROT, Item("Carrot", CAT_FOOD, 7, 0, MAX_STACK, FOOD, 2)},
	{ROASTED_CARROT, Item("Roasted Carrot", CAT_FOOD, 7, 1, MAX_STACK, FOOD, 4)},

	{EGG, Item("Egg", CAT_FOOD, 8, 0)},
	{FRIED_EGG, Item("Fried Egg", CAT_FOOD, 8, 1, MAX_STACK, FOOD, 4)},

	{APPLE, Item("Apple", CAT_FOOD, 9, 0, MAX_STACK, FOOD, 2)},
	{ROASTED_APPLE, Item("Roasted Apple", CAT_FOOD, 9, 1, MAX_STACK, FOOD, 4)},
};

const Item& ItemDef::Get(ItemID id)
{
	auto it = defs.find(id);
	if(it != defs.end()) {
		return it->second;
	}
	return defs.at(ITEMERR);
}

bool Item::CallItemAction(ItemID itemID, PlayerController* playerController, PlayerData* playerData, ChunkManager* chunkManager, Vector3Int targetBlockPos) {
	const Item& def = ItemDef::Get(itemID);
	if(def.itemType == FOOD && (playerData->GetHunger() < HUNGER_MAX || (playerData->GetHealth() < HEALTH_MAX && playerData->GetSaturation()<=0 ))) {
		playerData->ChangeHunger(def.tier);
		playerData->GetInventory()->SubHeldItem(); // this could cause logic errors when using an item that isnt held. But i dont see that happening
		Audio::Play("eat", 1.f);
	}

	auto it = itemActions.find(itemID);
	if(it != itemActions.end()) {
		it->second.Invoke({ playerController, playerData, chunkManager, targetBlockPos });
		return true;
	}
	return false;
}
