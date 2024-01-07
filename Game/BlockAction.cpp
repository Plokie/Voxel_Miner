#include "BlockAction.h"

#include "../Engine/Engine.h"
#include "InventoryUI.h"
#include "Blocks.h"
#include "ChunkManager.h"
#include "CraftingUI.h"

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
		ctx.invUI->Open(new CraftingUI(), InterfaceContext(ctx.inventory, nullptr, ctx.blockPosition, FURNACE));
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
