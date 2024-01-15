#include "CreativeUI.h"

#include "../Engine/UI/TextInput.h"

void CreativeUI::Open(InterfaceContext ctx) {
	TableInterface::Open(ctx);

	TextInput* textInput = MakeTextInput("Search");
	//textInput->SetPosition(100.f, 100.f);
	Button* testButton = MakeButton("Category");
	testButton->SetPosition(0.f, 40.f);


	const float spacing = 5.f;

	for(int y = 0; y < 3; y++) {
		for(int x = 0; x < 10; x++) {
			//Button* slot = MakeSlot(x, y); //todo: easier make slot func
			//slot->SetAnchor(0.f, 0.f);
			//slot->SetPivot(0.f, 0.f);
			//slot->SetPosition(
			//	x * (55.f + spacing) + 7.f,
			//	y * (55.f + spacing) + 80.f
			//);

			Button* button = new Button("Baloo");
			button->Init(pDevice);
			button->SetRect({ 0.3f,.3f,.3f,1.f });
			button->SetDimensions({ 50.f, 50.f });
			button->SetParent(this);
			button->SetAnchor(0.f, 0.f);
			button->SetPivot(0.f, 0.f);
			button->SetDepth(14.f);
			button->SetPosition(
				x * (50.f + spacing) + 0.f,
				y * (50.f + spacing) + 80.f
			);
			button->SetText("");

			_spawnedButtons.push_back(button);

			//TryMakeItemIcon(slot, x, y);
		}
	}
}

void CreativeUI::Close() {
	TableInterface::Close();
}

void CreativeUI::Start() {
}

void CreativeUI::Update(const float dt) {
	TableInterface::Update(dt);
}

void CreativeUI::Draw(SpriteBatch* spriteBatch) {
	TableInterface::Draw(spriteBatch);
}
