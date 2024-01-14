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
class BlockData;
class TableInterface;
class PlayerData;

struct BlockActionContext {
	PlayerController* playerController;
	PlayerData* playerData;
	ChunkManager* chunkManager;
	Vector3Int blockPosition;
	InventoryUI* invUI = nullptr;

	Chunk* GetChunk();
};

struct BlockTickContext {
	ChunkManager* chunkManager;
	Vector3Int blockPosition;
	Chunk* chunk;
	BlockData* blockData;
};

class BlockAction {
private:

	function<void(BlockActionContext)> func;
	static map<BlockID, BlockAction> blockActions;
public:

	void Invoke(BlockActionContext ctx);
	static bool CallBlockAction(BlockID blockID, PlayerController* playerController, PlayerData* playerData, ChunkManager* chunkManager, Vector3Int blockPos);

	BlockAction(function<void(BlockActionContext)> func);
};

class BlockTick {
private:

	function<void(BlockTickContext)> func;
	static map<BlockID, BlockTick> blockUpdates;

public:
	void Invoke(BlockTickContext ctx);
	static bool CallBlockTick(BlockID blockID, ChunkManager* chunkManager, Vector3Int blockPos, Chunk* chunk, BlockData* blockData);

	BlockTick(function<void(BlockTickContext)> func);
};