#include "LootTables.h"

map<string, LootTable> LootTable::map = {
	{"STONE", {{
		{COBBLESTONE, 1}
	}}},
	{"BLACKSTONE", {{
		{BLACK_COBBLESTONE, 1} //id, weight
	}}},
	{"COAL", {{
		{COAL, 1, 1, 2} //id, weight, min, max
	}}},
	{"COPPER", {{
		{RAW_COPPER, 1, 1, 2}
	}}},
	{"GOLD", {{
		{RAW_GOLD, 1}
	}}},
	{"AMETHYST", {{
		{AMETHYST, 1, 1, 2}
	}}},
	{"TITANIUM", {{
		{RAW_TITANIUM, 1}
	}}},
	{"LIT_FURNACE", {{
		{FURNACE, 1}
	}}},

	{"OAK_LEAVES", {{
		{OAK_LEAVES, 10},
		{APPLE, 1},
	}}},
	{"BIRCH_LEAVES", {{
		{BIRCH_LEAVES, 10},
		{APPLE, 1},
	}}},
	{"SPRUCE_LEAVES", {{
		{SPRUCE_LEAVES, 10},
		{APPLE, 1},
	}}},
	{"CHERRY_LEAVES", {{
		{CHERRY_LEAVES, 10},
		{APPLE, 1},
	}}},

	//{"STONE", LootTable({
	//	TableElement(COBBLESTONE, 1)
	//})}
};

InventoryItem LootTable::Choose(const string& tableName)
{
	auto it = map.find(tableName);
	if(it != map.end()) {
		LootTable& table = it->second;

		int weightSum = 0;
		for(const TableElement& element : table.elements) {
			weightSum += element.weight;
		}

		int randNum = rand() % weightSum;
		for(const TableElement& element : table.elements) {
			if(randNum < element.weight) {
				int range = element.maxAmt - element.minAmt + 1;
				int randAmount = rand() % range + element.minAmt;
				//int randAmount = (rand() % ((element.max + 1) - element.min)) + element.min;

				return InventoryItem(element.type, element.ID, -1, -1, randAmount);
			}
			randNum -= element.weight;
		}
	}

	return InventoryItem(InventoryItem::Type::BLOCK, 0, 0, 0, 1);
}
