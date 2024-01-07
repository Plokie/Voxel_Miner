#include "FurnaceUI.h"

#include "../Graphics/Graphics.h"

void FurnaceUI::Open(InterfaceContext ctx) {
	TableInterface::Open(ctx);
	
	Button* slot = MakeSlot(0, 0);
	slot->SetPosition(60.f, 60.f);

	//Button* slotRect = (Button*)gameScene->CreateObject2D(new Button(XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f)), "slot-" + to_string(row) + "-" + to_string(slot));
	//slotRect->SetDimensions({ 55.f, 55.f });
	//slotRect->SetDepth(12.f);
}

void FurnaceUI::Close() {
	TableInterface::Close();
}

void FurnaceUI::Start() {
}

void FurnaceUI::Update(const float dt){
	TableInterface::Update(dt);
}

void FurnaceUI::Draw(SpriteBatch* spriteBatch) {
	TableInterface::Draw(spriteBatch);
}
