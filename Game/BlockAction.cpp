#include "BlockAction.h"

#include "../Engine/Engine.h"
#include "InventoryUI.h"
#include "Blocks.h"
#include "ChunkManager.h"
#include "CraftingUI.h"
#include "FurnaceUI.h"
#include "ChestUI.h"
#include "Chunk.h"
#include "BlockData.h"
#include "ChunkDatabase.h"

//todo: move to recipes
static map<ItemID, ItemID> cookables = {
	{RAW_STEAK, COOKED_STEAK},
	{RAW_CHICKEN, COOKED_CHICKEN},
	{CARROT, ROASTED_CARROT},
	{EGG, FRIED_EGG},
	{APPLE, ROASTED_APPLE},

	{RAW_COPPER, COPPER_BAR},
	{RAW_GOLD, GOLD_BAR},
	{RAW_TITANIUM, TITANIUM_BAR},
};

map<BlockID, BlockAction> BlockAction::blockActions = {
	{WORKBENCH, {[](BlockActionContext ctx) {
		// code here is called when the block is clicked
		ctx.invUI->Open(new CraftingUI(), InterfaceContext(ctx.inventory, nullptr, ctx.blockPosition, WORKBENCH));
	}}},
	{FURNACE, {[](BlockActionContext ctx) {
		// code here is called when the block is clicked

		BlockData* blockData = nullptr;
		Chunk* chunk = ctx.GetChunk();
		tuple<int, int, int> chunkIndex = ctx.chunkManager->ToChunkIndexPositionTuple(ctx.blockPosition.x, ctx.blockPosition.y, ctx.blockPosition.z);
		if(chunk) {
			auto findIt = chunk->blockDataData.find(ctx.blockPosition);
			if(findIt != chunk->blockDataData.end()) {
				blockData = findIt->second;
			}

			if(blockData == nullptr) {
				blockData = new BlockData(new Inventory());
				chunk->blockDataData[ctx.blockPosition] = blockData;
				ChunkDatabase::Get()->SaveChunkData(chunkIndex, chunk);
			}
			ctx.invUI->Open(new FurnaceUI(), InterfaceContext(ctx.inventory, blockData, ctx.blockPosition, FURNACE));
		}
	}}},
	{CHEST, {[](BlockActionContext ctx) {
		// code here is called when the block is clicked

		BlockData* blockData = nullptr;
		Chunk* chunk = ctx.GetChunk();
		tuple<int, int, int> chunkIndex = ctx.chunkManager->ToChunkIndexPositionTuple(ctx.blockPosition.x, ctx.blockPosition.y, ctx.blockPosition.z);
		if(chunk) {
			auto findIt = chunk->blockDataData.find(ctx.blockPosition);
			if(findIt != chunk->blockDataData.end()) {
				blockData = findIt->second;
			}

			if(blockData == nullptr) {
				blockData = new BlockData(new Inventory());
				chunk->blockDataData[ctx.blockPosition] = blockData;
				ChunkDatabase::Get()->SaveChunkData(chunkIndex, chunk);
			}
			ctx.invUI->Open(new ChestUI(), InterfaceContext(ctx.inventory, blockData, ctx.blockPosition, CHEST));
		}
	}}},
};

bool BlockAction::CallBlockAction(BlockID blockID, PlayerController* playerController, Inventory* inv, ChunkManager* chunkManager, Vector3Int blockPos) {
	auto it = blockActions.find(blockID);
	if(it != blockActions.end()) {
		it->second.Invoke({ playerController, inv, chunkManager, blockPos });
		return true;
	}
	return false;
}

void BlockAction::Invoke(BlockActionContext ctx) {
	ctx.invUI = Engine::Get()->GetCurrentScene()->GetObject2D<InventoryUI>("invUI");
	func(ctx);
}

BlockAction::BlockAction(function<void(BlockActionContext)> func) {
	this->func = func;
}

Chunk* BlockActionContext::GetChunk() {
	tuple<int, int, int> chunkIndex = this->chunkManager->ToChunkIndexPositionTuple(blockPosition.x, blockPosition.y, blockPosition.z);
	return chunkManager->GetChunk(chunkIndex);
}
