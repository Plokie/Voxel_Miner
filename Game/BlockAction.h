#pragma once

#include <functional>
#include "../Engine/MathUtil.h"

using namespace std;

class PlayerController;
class Inventory;
class ChunkManager;

struct BlockActionContext {
	PlayerController* playerController;
	Inventory* inventory;
	ChunkManager* chunkManager;
	Vector3Int blockPosition;
};

class BlockAction {
private:

	function<void(BlockActionContext)> func;

public:

	void Invoke(BlockActionContext ctx);

	BlockAction(function<void(BlockActionContext)> func);
};