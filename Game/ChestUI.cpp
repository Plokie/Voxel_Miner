#include "ChestUI.h"

#include "../Graphics/Graphics.h"
#include "BlockData.h"
#include "Inventory.h"
#include "ItemIcon.h"

void ChestUI::Open(InterfaceContext ctx) {
	TableInterface::Open(ctx);

	const float spacing = 5.f;

	for(int y = 0; y < CHESTSIZE_Y; y++) {
		for(int x = 0; x < CHESTSIZE_X; x++) {
			Button* slot = MakeSlot(x, y); //todo: easier make slot func
			slot->SetAnchor(0.f, 0.f);
			slot->SetPivot(0.f, 0.f);
			slot->SetPosition(
				x * (55.f + spacing) + 7.f, 
				y * (55.f + spacing) + 55.f
			);

			TryMakeItemIcon(slot, x, y);
		}
	}

	//Button* slotRect = (Button*)gameScene->CreateObject2D(new Button(XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f)), "slot-" + to_string(row) + "-" + to_string(slot));
	//slotRect->SetDimensions({ 55.f, 55.f });
	//slotRect->SetDepth(12.f);
}

void ChestUI::Close() {
	TableInterface::Close();
}

void ChestUI::Start() {
}

void ChestUI::Update(const float dt) {
	TableInterface::Update(dt);
}

void ChestUI::Draw(SpriteBatch* spriteBatch) {
	TableInterface::Draw(spriteBatch);
}