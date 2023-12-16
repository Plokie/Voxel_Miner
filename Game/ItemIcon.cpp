#include "ItemIcon.h"

#include "../Engine/UI/Label.h"
#include "../Engine/UI/UIRect.h"
#include "../Engine/Engine.h"
#include "InventoryUI.h"
#include "Inventory.h"
#include "Crafting.h"

ItemIcon::ItemIcon(InventoryItem* invItem, InventoryUI* invUI) {
	this->invUI = invUI;
	this->invItem = invItem;
	Display(invItem->ID, invItem->type, invItem->amount, invItem->GetUVPos());
	/*const string& atlasName = (invItem->type == InventoryItem::Type::BLOCK) ? "atlas" : "item-atlas";

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


	nameLabel = new Label(L"Data\\Fonts\\Baloo2.spritefont", { 1.f,1.f,1.f,1.f });
	nameLabel->Init(Graphics::Get()->GetDevice());
	nameLabel->InitSelf();
	nameLabel->SetText((invItem->type == InventoryItem::Type::BLOCK) ? (BlockDef::GetDef((BlockID)invItem->ID).GetName() ) : (ItemDef::Get((ItemID)invItem->ID).GetName()));
	nameLabel->SetAnchor({ .5f, 0.f });
	nameLabel->SetPivot({ .5f, .5f });
	nameLabel->SetPosition({ .0f, 0.0f });
	nameLabel->SetDepth(20.f);
	nameLabel->SetParent(icon);
	nameLabel->SetEnabled(false);*/
}

ItemIcon::ItemIcon(const RecipeComponent& recipeComponent) {
	Display(recipeComponent.ID, recipeComponent.type, recipeComponent.amount, recipeComponent.GetUVPos(), false);
}

ItemIcon::~ItemIcon() {
	if(icon != nullptr) {
		delete icon;
	}
	if(amtLabel != nullptr) {
		delete amtLabel;
	}
}

void ItemIcon::Display(const unsigned short ID, const InventoryItem::Type type, const int amount, const Vector2Int& uvPos, const bool hideOne) {
	const string& atlasName = (type == InventoryItem::Type::BLOCK) ? "atlas" : "item-atlas";

	icon = new UIRect(atlasName, { 1.f,1.f,1.f,1.f });
	icon->Init(Graphics::Get()->GetDevice());
	icon->SetDimensions({ 50.f,50.f });
	icon->SetAnchor({ .5f,.5f });
	icon->SetPivot({ .5f,.5f });
	icon->SetDepth(13.f);
	icon->SetParent(this);

	//Vector2Int uvPos = invItem->GetUVPos();
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
	amtLabel->SetText((amount == 1 && hideOne) ? "" : to_string(amount));
	amtLabel->SetAnchor({ 1.f,1.f });
	amtLabel->SetPivot({ 1.f,1.f });
	amtLabel->SetPosition({ .0f, 12.0f });
	amtLabel->SetDepth(14.f);
	amtLabel->SetParent(icon);


	nameLabel = new Label(L"Data\\Fonts\\Baloo2.spritefont", { 1.f,1.f,1.f,1.f });
	nameLabel->Init(Graphics::Get()->GetDevice());
	nameLabel->InitSelf();
	nameLabel->SetText((type == InventoryItem::Type::BLOCK) ? (BlockDef::GetDef((BlockID)ID).GetName()) : (ItemDef::Get((ItemID)ID).GetName()));
	nameLabel->SetAnchor({ .5f, 0.f });
	nameLabel->SetPivot({ .5f, .5f });
	nameLabel->SetPosition({ .0f, 0.0f });
	nameLabel->SetDepth(20.f);
	nameLabel->SetParent(icon);
	nameLabel->SetEnabled(false);
}

const bool ItemIcon::WasPressed() {
	return isHovering && Input::IsMouseKeyPressed(MOUSE_L);
}

const bool ItemIcon::WasRightClicked()
{
	return isHovering && Input::IsMouseKeyPressed(MOUSE_R);
}

const Vector2 ItemIcon::GetScreenPosition()
{
	if(isHeld) {
		XMFLOAT2 mPos = Input::MousePosition();
		icon->SetDepth(15.f);
		amtLabel->SetDepth(16.f);
		return Vector2( mPos.x, mPos.y ) - (this->dimensions / 2.f);
	}
	else {
		icon->SetDepth(13.f);
		amtLabel->SetDepth(14.f);
		return Object2D::GetScreenPosition();
	}
}

void ItemIcon::Draw(SpriteBatch* spriteBatch) {
	if(icon != nullptr) {
		icon->Draw(spriteBatch);
	}
	if(amtLabel != nullptr) {
		amtLabel->Draw(spriteBatch);
	}
	if(nameLabel != nullptr && nameLabel->GetEnabled()) {
		nameLabel->Draw(spriteBatch);
	}
}

void ItemIcon::SetDimensions(const Vector2& dim) {
	this->dimensions = dim;
	if(icon != nullptr) {
		icon->SetDimensions(dim);
	}
}

void ItemIcon::Update(const float dTime) {
	if(invUI == nullptr) return;
	XMFLOAT2 mPos = Input::MousePosition();
	Vector2 sPos = GetScreenPosition();

	isHovering =
		mPos.x < sPos.x + dimensions.x &&
		mPos.x > sPos.x &&
		mPos.y < sPos.y + dimensions.y &&
		mPos.y > sPos.y;
	
	nameLabel->SetEnabled(isHovering);

	if(WasPressed()) {
		if(Input::IsKeyHeld(VK_SHIFT)) {
			Vector2Int freeSpot = invUI->GetInventory()->GetFreeSpot();
			invItem->posX = freeSpot.x;
			invItem->posY = freeSpot.y;
			SetParent((Object2D*)invUI->GetInvSlot(freeSpot.x, freeSpot.y));

			invUI->DrawHotbarIcons();
		}
		else {
			isHeld = !isHeld;

			if(!isHeld) { // If released
				invUI->ReleaseItem(this);
			}
			else { // If begin held
				invUI->heldItem = this;
			}
		}

	}

	if(WasRightClicked() && invUI->heldItem!=this) {
		Inventory* inv = Engine::Get()->GetCurrentScene()->GetObject3D<Inventory>("Inventory");
		
		int splitAmount = 1;
		if(Input::IsKeyHeld(VK_SHIFT)) splitAmount = invItem->amount / 2;

		invItem->amount -= splitAmount;
		amtLabel->SetText((invItem->amount == 1 /*&& hideOne*/) ? "" : to_string(invItem->amount));

		if(invItem->amount <= 0) {
			inv->ClearEmptyItems();
		}
		
		if(invUI->heldItem != nullptr) {
			if(invUI->heldItem->invItem->ID == invItem->ID && invUI->heldItem->invItem->type == invItem->type) {
				invUI->heldItem->invItem->amount += splitAmount;
				invUI->heldItem->amtLabel->SetText((invUI->heldItem->invItem->amount == 1 /*&& hideOne*/) ? "" : to_string(invUI->heldItem->invItem->amount));
			}
		}
		else {
			InventoryItem* tempInvItem = new InventoryItem(invItem->type, invItem->ID, -1, -1, splitAmount); //-1, -1 flags that its a temp object and to-be deleted when icon released (ig this is unsafe but aghhh idk)
			//inv->AddItem(invItem->ID, invItem->type, splitAmount);
			//InventoryItem* invItemTarget
			//inv->AddItem()

			ItemIcon* newIcon = new ItemIcon(tempInvItem, invUI);
			newIcon->Init(pDevice);
			newIcon->SetDimensions({ 50.f, 50.f });
			newIcon->SetAnchor({ .5f,.5f });
			newIcon->SetPivot({ .5f,.5f });
			newIcon->isHeld = true;
			invUI->heldItem = newIcon;

			invUI->AddNewIcon(newIcon);
		}
	}
}
