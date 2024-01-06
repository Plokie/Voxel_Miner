#include "BlockAction.h"

void BlockAction::Invoke(BlockActionContext ctx) {
	func(ctx);
}

BlockAction::BlockAction(function<void(BlockActionContext)> func) {
	this->func = func;
}
