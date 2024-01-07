#pragma once

#include <functional>
#include "../Engine/MathUtil.h"
#include "Blocks.h"

using namespace std;

class PlayerController;
class Inventory;
class ChunkManager;
class InventoryUI;
class Chunk;

struct BlockActionContext {
	PlayerController* playerController;
	Inventory* inventory;
	ChunkManager* chunkManager;
	Vector3Int blockPosition;
	InventoryUI* invUI = nullptr;

	Chunk* GetChunk();
};

class BlockAction {
private:

	function<void(BlockActionContext)> func;
	static map<BlockID, BlockAction> blockActions;
public:

	void Invoke(BlockActionContext ctx);
	static bool CallBlockAction(BlockID blockID, PlayerController* playerController, Inventory* inv, ChunkManager* chunkManager, Vector3Int blockPos);

	BlockAction(function<void(BlockActionContext)> func);
};