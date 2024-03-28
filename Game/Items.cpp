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

	{COPPER_PICKAXE, Item("Copper Pickaxe", CAT_UTILITY, 1, 2, 1, IT_PICKAXE | (1<<TIER_TAG)) },
	{GOLD_PICKAXE, Item("Gold Pickaxe", CAT_UTILITY, 2, 2, 1, IT_PICKAXE | (2 << TIER_TAG))},
	{AMETHYST_PICKAXE, Item("Amethyst Pickaxe", CAT_UTILITY, 3, 2, 1, IT_PICKAXE | (3 << TIER_TAG))},
	{TITANIUM_PICKAXE, Item("Titanium Pickaxe", CAT_UTILITY, 4, 2, 1, IT_PICKAXE | (4 << TIER_TAG))},

	{COPPER_AXE, Item("Copper Axe", CAT_UTILITY, 1, 3, 1, IT_AXE | (1 << TIER_TAG))},
	{GOLD_AXE, Item("Gold Axe", CAT_UTILITY, 2, 3, 1, IT_AXE | (2 << TIER_TAG))},
	{AMETHYST_AXE, Item("Amethyst Axe", CAT_UTILITY, 3, 3, 1, IT_AXE | (3 << TIER_TAG))},
	{TITANIUM_AXE, Item("Titanium Axe", CAT_UTILITY, 4, 3, 1, IT_AXE | (4 << TIER_TAG))},

	{COPPER_SHOVEL, Item("Copper Shovel", CAT_UTILITY, 1, 4, 1, IT_SHOVEL | (1 << TIER_TAG))},
	{GOLD_SHOVEL, Item("Gold Shovel", CAT_UTILITY, 2, 4, 1, IT_SHOVEL | (2 << TIER_TAG))},
	{AMETHYST_SHOVEL, Item("Amethyst Shovel", CAT_UTILITY, 3, 4, 1, IT_SHOVEL | (3 << TIER_TAG))},
	{TITANIUM_SHOVEL, Item("Titanium Shovel", CAT_UTILITY, 4, 4, 1, IT_SHOVEL | (4 << TIER_TAG))},

	{STICK, Item("Stick", CAT_MISC, 0, 2)},

	{RAW_STEAK, Item("Raw Steak", CAT_FOOD, 5, 0, MAX_STACK, IT_FOOD | (3 << TIER_TAG))},
	{COOKED_STEAK, Item("Cooked Steak", CAT_FOOD, 5, 1, MAX_STACK, IT_FOOD | (6 << TIER_TAG))},

	{RAW_CHICKEN, Item("Raw Chicken", CAT_FOOD, 6, 0, MAX_STACK, IT_FOOD | (2 << TIER_TAG))},
	{COOKED_CHICKEN, Item("Cooked Chicken", CAT_FOOD, 6, 1, MAX_STACK, IT_FOOD | (7 << TIER_TAG))},

	{CARROT, Item("Carrot", CAT_FOOD, 7, 0, MAX_STACK, IT_FOOD | (2 << TIER_TAG))},
	{ROASTED_CARROT, Item("Roasted Carrot", CAT_FOOD, 7, 1, MAX_STACK, IT_FOOD | (4 << TIER_TAG))},

	{EGG, Item("Egg", CAT_FOOD, 8, 0)},
	{FRIED_EGG, Item("Fried Egg", CAT_FOOD, 8, 1, MAX_STACK, IT_FOOD | (4 << TIER_TAG))},

	{APPLE, Item("Apple", CAT_FOOD, 9, 0, MAX_STACK, IT_FOOD | (3 << TIER_TAG))},
	{ROASTED_APPLE, Item("Roasted Apple", CAT_FOOD, 9, 1, MAX_STACK, IT_FOOD | (4 << TIER_TAG))},
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
	if(def.tags&IT_FOOD && (playerData->GetHunger() < HUNGER_MAX || (playerData->GetHealth() < HEALTH_MAX && playerData->GetSaturation()<=0 ))) {
		playerData->ChangeHunger(def.GetTier());
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
