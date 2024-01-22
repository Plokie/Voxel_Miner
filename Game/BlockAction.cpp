#include "BlockAction.h"

#include "BlockData.h"
#include "../Engine/Engine.h"
#include "InventoryUI.h"
#include "Blocks.h"
#include "ChunkManager.h"
#include "CraftingUI.h"
#include "Crafting.h"
#include "FurnaceUI.h"
#include "ChestUI.h"
#include "Chunk.h"
#include "BlockData.h"
#include "ChunkDatabase.h"
#include "PlayerData.h"

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

void FurnaceActionFunction(BlockActionContext ctx) {
	BlockData* blockData = nullptr;
	Chunk* chunk = ctx.GetChunk();
	tuple<int, int, int> chunkIndex = ctx.chunkManager->ToChunkIndexPositionTuple(ctx.blockPosition.x, ctx.blockPosition.y, ctx.blockPosition.z);
	if(chunk) {
		auto findIt = chunk->blockDataData.find(ctx.blockPosition);
		if(findIt != chunk->blockDataData.end()) {
			blockData = findIt->second;
		}

		if(blockData == nullptr /*|| blockData->blockInventory == nullptr*/) {
			blockData = new BlockData(new Inventory());
			chunk->blockDataData[ctx.blockPosition] = blockData;
		}
		ctx.invUI->Open(new FurnaceUI(), InterfaceContext(ctx.playerData, blockData, ctx.blockPosition, FURNACE));
		ChunkDatabase::Get()->SaveChunkData(chunkIndex, chunk);
	}
}

map<BlockID, BlockAction> BlockAction::blockActions = {
	{WORKBENCH, {[](BlockActionContext ctx) {
		// code here is called when the block is clicked
		ctx.invUI->Open(new CraftingUI(), InterfaceContext(ctx.playerData, nullptr, ctx.blockPosition, WORKBENCH));
	}}},
	{FURNACE, {[](BlockActionContext ctx) {
		// code here is called when the block is clicked

		FurnaceActionFunction(ctx);
	}}},
	{LIT_FURNACE, {[](BlockActionContext ctx) {
		// code here is called when the block is clicked

		FurnaceActionFunction(ctx);
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

			if(blockData == nullptr || blockData->blockInventory == nullptr) {
				blockData = new BlockData(new Inventory());
				chunk->blockDataData[ctx.blockPosition] = blockData;
			}
			ctx.invUI->Open(new ChestUI(), InterfaceContext(ctx.playerData, blockData, ctx.blockPosition, CHEST));
			ChunkDatabase::Get()->SaveChunkData(chunkIndex, chunk);
		}
	}}},
};

void FurnaceUpdateFunction(BlockTickContext ctx) {
	const float secondsPerTick = 0.05f;

	//timer1 = fuel timer. decrements from 80 seconds to zero
	//timer2 = output progress timer. increments.

	if(ctx.blockData->blockInventory) {
		InventoryItem* inputItem;
		bool hasInputItem = ctx.blockData->blockInventory->GetItemAt(1, 0, &inputItem);
		const RecipeComponent* resultRecipe = nullptr;
		bool isInputItemValid = false;

		if(hasInputItem)
			isInputItemValid = Crafting::GetFurnaceRecipe(inputItem, &resultRecipe);

		InventoryItem* outputItemSlot;
		bool hasOutputItem = ctx.blockData->blockInventory->GetItemAt(2, 0, &outputItemSlot);

		if(ctx.blockData->timer1 > 0.f) { // If coal is currently burning
			//ctx.chunkManager->SetBlockAtWorldPos(ctx.blockPosition, LIT_FURNACE);
			//if(ctx.chunk->blockData[ctx.])

			ctx.blockData->timer1 -= secondsPerTick;

			if(ctx.blockData->timer1 <= 0.f) {
				// if coal has JUST ran out


				ctx.chunkManager->SetBlockAtWorldPos(ctx.blockPosition, FURNACE);


			}

			if(hasInputItem && isInputItemValid) {
				ctx.blockData->timer2 += secondsPerTick;
				if(ctx.blockData->timer2 > 10.f) { // If the current smelting progress is complete


					if(hasOutputItem) {
						if(outputItemSlot->ID != resultRecipe->ID || outputItemSlot->type != resultRecipe->type) {

							// if there is already an item in the output slot that does not match the desired output
							// chicken out
							return;
						}
					}

					// convert input to an output
					ctx.blockData->blockInventory->SubItem(inputItem->ID, inputItem->type);

					InventoryItem* invItemResult = nullptr;
					ctx.blockData->blockInventory->AddItem(resultRecipe->ID, resultRecipe->type, resultRecipe->amount, &invItemResult);

					if(invItemResult) {
						invItemResult->posX = 2; // force to output slot
					}
					//ctx.blockData->blockInventory->PushItem(new InventoryItem(resultRecipe->ID))

					ctx.blockData->timer2 = 0.f;

					//Reload icons
					Engine::Get()->GetCurrentScene()->GetObject2D<InventoryUI>("invUI")->TableReload(ctx.blockPosition);
				}
			}
			else {
				ctx.blockData->timer2 = 0.f; //Reset progress if no item in input slot
			}
		}
		else { // if fuel has expired

			if(isInputItemValid) {
				InventoryItem* fuel;
				if(ctx.blockData->blockInventory->GetItemAt(0, 0, &fuel)) {
					if(fuel->Is(COAL)) { // check if coal is in input slot
						ctx.blockData->timer1 = 80.f; // replenish fuel timer


						ctx.chunkManager->SetBlockAtWorldPos(ctx.blockPosition, LIT_FURNACE);


						ctx.blockData->blockInventory->SubItem(COAL);
						//Reload icons
						Engine::Get()->GetCurrentScene()->GetObject2D<InventoryUI>("invUI")->TableReload(ctx.blockPosition);
					}
				}

			}
		}
	}
}

map<BlockID, BlockTick> BlockTick::blockUpdates = {
	{FURNACE, {[](BlockTickContext ctx) {
		// code here is called every chunk tick (20 times per second, 0.05 seconds per tick)

		FurnaceUpdateFunction(ctx);
	}}},
	{LIT_FURNACE, {[](BlockTickContext ctx) {
		// code here is called every chunk tick (20 times per second, 0.05 seconds per tick)

		FurnaceUpdateFunction(ctx);
	}}}
};

bool BlockAction::CallBlockAction(BlockID blockID, PlayerController* playerController, PlayerData* playerData, ChunkManager* chunkManager, Vector3Int blockPos) {
	auto it = blockActions.find(blockID);
	if(it != blockActions.end()) {
		it->second.Invoke({ playerController, playerData, chunkManager, blockPos });
		return true;
	}
	return false;
}

void BlockTick::Invoke(BlockTickContext ctx) {
	func(ctx);
}

bool BlockTick::CallBlockTick(BlockID blockID, ChunkManager* chunkManager, Vector3Int blockPos, Chunk* chunk, BlockData* blockData)
{
	auto it = blockUpdates.find(blockID);
	if(it != blockUpdates.end()) {
		it->second.Invoke({ chunkManager, blockPos, chunk, blockData });
		return true;
	}
	return false;
}

BlockTick::BlockTick(function<void(BlockTickContext)> func) {
	this->func = func;
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
