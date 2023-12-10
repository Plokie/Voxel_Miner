#pragma once

#include <map>
#include <string>
#include <vector>

#include "Items.h"
#include "Blocks.h"
#include "InventoryItem.h"

using namespace std;

class TableElement {
private:
public:
	unsigned short ID=0;
	InventoryItem::Type type= InventoryItem::Type::BLOCK;
	int weight=1;
	int minAmt=1, maxAmt=1; //min : inclusive, max : inclusive
	
	TableElement(ItemID itemID, int weight) : ID(itemID), weight(weight), type(InventoryItem::Type::ITEM) {}
	TableElement(BlockID blockID, int weight) : ID(blockID), weight(weight), type(InventoryItem::Type::BLOCK) {}
	TableElement(ItemID itemID, int weight, int minAmt, int maxAmt) : ID(itemID), weight(weight), minAmt(minAmt), maxAmt(maxAmt), type(InventoryItem::Type::ITEM) {}
	TableElement(BlockID blockID, int weight, int minAmt, int maxAmt) : ID(blockID), weight(weight), minAmt(minAmt), maxAmt(maxAmt), type(InventoryItem::Type::BLOCK) {}
};


class LootTable {
	static map<string, LootTable> map;
private:
	vector<TableElement> elements = {};


	LootTable(vector<TableElement> elements) : elements(elements) {}
public:

	static InventoryItem Choose(const string& tableName);
};