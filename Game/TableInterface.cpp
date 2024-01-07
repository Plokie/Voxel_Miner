#include "TableInterface.h"

#include "../Engine/UI/Button.h"
#include "ItemIcon.h"
#include "../Graphics/Graphics.h"

void TableInterface::Close()
{
	for(auto& slot : slots) {
		delete slot;
		slot = nullptr;
	}

	for(auto& itemIcon : _spawnedItemIcons) {
		delete itemIcon;
		itemIcon = nullptr;
	}
}

void TableInterface::Start() {
	this->Init(Graphics::Get()->GetDevice());
	this->InitSelf(); //overriden func to init child-specific features

	this->SetPivot(0.5f, 0.0f);
	this->SetAnchor(0.5f, 0.f);
	this->SetDimensions({ 550.f, 265.f });
	this->SetPosition(0.f, 10.f);
	this->SetDepth(11.f);
}
