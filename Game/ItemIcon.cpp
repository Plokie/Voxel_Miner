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
	this->inv = Engine::Get()->GetCurrentScene()->GetObject3D<Inventory>("Inventory");
	Display(invItem->ID, invItem->type, invItem->amount, invItem->GetUVPos());
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
	if(nameLabel != nullptr) {
		delete nameLabel;
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


	amtLabel = new Label("Baloo", { 1.f,1.f,1.f,1.f });
	amtLabel->Init(Graphics::Get()->GetDevice());
	amtLabel->InitSelf();
	amtLabel->SetText((amount == 1 && hideOne) ? "" : to_string(amount));
	amtLabel->SetAnchor({ 1.f,1.f });
	amtLabel->SetPivot({ 1.f,1.f });
	amtLabel->SetPosition({ .0f, 12.0f });
	amtLabel->SetDepth(14.f);
	amtLabel->SetParent(icon);


	nameLabel = new Label("Baloo", { 1.f,1.f,1.f,1.f });
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

void ItemIcon::Reload() {
	if(invItem != nullptr) {
		// hmm smells like one or more of these delete functions dont properly delete everythign
		if(amtLabel) delete amtLabel;
		if(nameLabel) delete nameLabel;
		if(icon) delete icon;

		Display(invItem->ID, invItem->type, invItem->amount, invItem->GetUVPos());
	}
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
		if(invUI->heldItem == nullptr || invUI->heldItem == this) {
			//isHeld = !isHeld;
			if(!isHeld) { // Begin held
				isHeld = true;
				invUI->heldItem = this;
			}
			else { // Release
				invUI->ReleaseItem(this);
			}
		}
	}

	if(WasRightClicked()) {
		if(invUI->heldItem == nullptr) {
			// Create a new held item
		}

		else if(invUI->heldItem->GetInvItem()->IsSameItemAs(invItem)) {
			// Add to the currently held item
		}
	}

	if(WasRightClicked() && invUI->heldItem != this) {
		int subAmt = Input::IsKeyHeld(VK_SHIFT) ? invItem->amount/2 : 1;


		if(invUI->heldItem) {
			if(invUI->heldItem->GetInvItem()->IsSameItemAs(invItem)) {
				// attempt to add one to held amt, sub one from this

				if(invUI->heldItem->GetInvItem()->amount < invUI->heldItem->GetInvItem()->GetMaxStack()) {
					invUI->heldItem->GetInvItem()->amount += subAmt;
					invItem->amount -= subAmt;

					if(invItem->amount <= 0) {
						
						invUI->EraseIcon(this);
						for(auto it = inv->GetInventoryItems().begin(); it != inv->GetInventoryItems().end(); ++it) {
							if(*it == invItem) {
								inv->GetInventoryItems().erase(it);
								break;
							}
						}
						//inv->ClearEmptyItems();

						delete invItem;

						invUI->ReloadIcons();
						delete this; //uh oh
						return; // should be okay as long as this return stays here
					}
				}

			}

			invUI->ReloadIcons();
		}
		else {
			if(invItem->amount == 1) {
				isHeld = true;
				invUI->heldItem = this;
			}
			else {
				// create new icon, set amt to 1, sub 1 from this
				invItem->amount -= subAmt;
				InventoryItem* newInvItem = new InventoryItem(invItem->type, invItem->ID, -1, -1, subAmt);
				inv->PushItem(newInvItem);

				ItemIcon* newIcon = new ItemIcon(newInvItem, invUI);
				newIcon->Init(pDevice);
				newIcon->SetDimensions({ 50.f, 50.f });
				newIcon->SetAnchor({ .5f,.5f });
				newIcon->SetPivot({ .5f,.5f });
				newIcon->isHeld = true;
				invUI->heldItem = newIcon;
				invUI->AddNewIcon(newIcon);
			}
			invUI->ReloadIcons();
		}

	}
}
