#include "ItemIcon.h"

#include "../Engine/UI/Label.h"
#include "../Engine/UI/UIRect.h"
#include "InventoryItem.h"
#include "../Engine/Engine.h"

ItemIcon::ItemIcon(const InventoryItem* invItem) {
	const string& atlasName = (invItem->type == InventoryItem::Type::BLOCK) ? "atlas" : "item-atlas";

	icon = new UIRect(atlasName, { 1.f,1.f,1.f,1.f });
	icon->Init(Graphics::Get()->GetDevice());
	icon->SetDimensions({ 50.f,50.f });
	icon->SetAnchor({ .5f,.5f });
	icon->SetPivot({ .5f,.5f });
	icon->SetDepth(13.f);
	icon->SetParent(this);

	Vector2Int uvPos = invItem->GetUVPos();
	LONG texPosX = static_cast<LONG>(ITEM_ATLAS_TILE_SIZE * uvPos.x);
	LONG texPosY = static_cast<LONG>(ITEM_ATLAS_TILE_SIZE * uvPos.y);

	icon->SetTexRect({
		texPosX, texPosY,
		texPosX + ITEM_ATLAS_TILE_SIZE,
		texPosY + ITEM_ATLAS_TILE_SIZE
	});

	
	amtLabel = new Label(L"Data\\Fonts\\Baloo2.spritefont", { 1.f,1.f,1.f,1.f });
	amtLabel->Init(Graphics::Get()->GetDevice());
	amtLabel->InitSelf();
	amtLabel->SetText((invItem->amount==1)?"":to_string(invItem->amount));
	amtLabel->SetAnchor({ 1.f,1.f });
	amtLabel->SetPivot({ 1.f,1.f });
	amtLabel->SetPosition({ .0f, 12.0f });
	amtLabel->SetDepth(14.f);
	amtLabel->SetParent(icon);

}

ItemIcon::~ItemIcon() {
	if(icon != nullptr) {
		delete icon;
	}
	if(amtLabel != nullptr) {
		delete amtLabel;
	}
}

void ItemIcon::Draw(SpriteBatch* spriteBatch) {
	if(icon != nullptr) {
		icon->Draw(spriteBatch);
	}
	if(amtLabel != nullptr) {
		amtLabel->Draw(spriteBatch);
	}
}

void ItemIcon::SetDimensions(const Vector2& dim) {
	this->dimensions = dim;
	if(icon != nullptr) {
		icon->SetDimensions(dim);
	}
}
