#include "ItemIcon.h"

#include "../Engine/UI/Label.h"
#include "../Engine/UI/UIRect.h"
#include "../Engine/UI/Button.h"
#include "../Engine/Engine.h"
#include "InventoryUI.h"
#include "Inventory.h"
#include "Crafting.h"
#include "TableInterface.h"
#include "PlayerData.h"

ItemIcon::ItemIcon(InventoryItem* invItem, InventoryUI* invUI) {
	this->invUI = invUI;
	this->invItem = invItem;
	this->inv = Engine::Get()->GetScene("game")->GetObject3D<PlayerData>("PlayerData")->GetInventory(); // todo: change this to point to correct inventory. pass as argument?
	//this->inv = nullptr;
	Display(invItem->ID, invItem->type, invItem->amount, invItem->GetUVPos());
}

ItemIcon::ItemIcon(const RecipeComponent& recipeComponent) {
	Display(recipeComponent.ID, recipeComponent.type, recipeComponent.amount, recipeComponent.GetUVPos(), false);
}

ItemIcon::ItemIcon(BlockID blockID) {
	const Block& blockDef = BlockDef::GetDef(blockID);
	Display(blockID, InventoryItem::BLOCK, 1, { blockDef.GetSideUVidx(), blockDef.GetSideUVidy() });
}

ItemIcon::ItemIcon(ItemID itemID)
{
	const Item& itemDef = ItemDef::Get(itemID);
	Display(itemID, InventoryItem::ITEM, 1, { itemDef.UVx(), itemDef.UVy() });
}

ItemIcon::ItemIcon(unsigned short ID, InventoryItem::Type type)
{
	if(type == InventoryItem::ITEM) {
		const Item& itemDef = ItemDef::Get((ItemID)ID);
		Display(ID, type, 1, { itemDef.UVx(), itemDef.UVy() });
	}
	else {
		const Block& blockDef = BlockDef::GetDef((BlockID)ID);
		Display(ID, type, 1, { blockDef.GetSideUVidx(), blockDef.GetSideUVidy() });
	}
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
		if(amtLabel) delete amtLabel;
		if(nameLabel) delete nameLabel;
		if(icon) delete icon;

		Display(invItem->ID, invItem->type, invItem->amount, invItem->GetUVPos());
	}
}

void ItemIcon::SetInventoryParent(Inventory* inv) {
	this->inv = inv;
}

void ItemIcon::AttemptStorageMigration(Inventory* slotParentInventory, TableInterface* currentInterface) {


	Inventory* playerInventory = Engine::Get()->GetCurrentScene()->GetObject3D<PlayerData>("PlayerData")->GetInventory();
	Inventory* currentInventory = inv;
	
	////targetInventory : the inventory that is being hovered over
	//////////tableInterface ??? : nullptr by default, if the ItemIcon belongs to a TableInterface, this is a pointer to that k
	////currentInterface : the currently active interface
	////playerInventory : the player's own inventory
	////currentInventory : the inventory that the ItemIcon currently belongs to

	if(currentInventory != slotParentInventory) {
		// if the inventories dont match, then a swap needs to be made

		if(currentInventory != playerInventory) { // if ItemIcon belongs to an interface
			currentInterface->EraseIcon(this); // Remove icon from interface
			invUI->AddNewIcon(this); // Move to default invUI
		}
		else { // if ItemIocn belongs to player inventory
			invUI->EraseIcon(this); // Remove icon from invUI
			currentInterface->PushIcon(this); // Move to interface UI
		}

		// erase the ItemIcon's InventoryItem* from its current inventory
		auto it = currentInventory->GetInventoryItems().begin(); // erase invitem from current inventory
		while(it != currentInventory->GetInventoryItems().end()) {
			if(*it == GetInvItem()) {
				currentInventory->GetInventoryItems().erase(it);
				break;
			}
			++it;
		}

		// now move the InventoryItem* to the target inventory
		slotParentInventory->PushItem(GetInvItem());
		SetInventoryParent(slotParentInventory);
		inv = slotParentInventory;

		onPlacedownCooldown = true;
	}
}

void ItemIcon::ReleaseFunction(int idx, int idy, Button* parentSlot, Inventory* slotParentInventory, TableInterface* tableInterface) {
	InventoryItem* thisInvItem = GetInvItem();
	InventoryItem* prexistingInvItem = nullptr;
	

	if(slotParentInventory->GetItemAt(idx, idy, &prexistingInvItem)) {
		
		
		if(prexistingInvItem == thisInvItem) {
			thisInvItem->posX = idx;
			thisInvItem->posY = idy;
			SetParent(parentSlot);
			isHeld = false;

			AttemptStorageMigration(slotParentInventory, tableInterface);

			invUI->heldItem = nullptr;
		}
		else {
			if(prexistingInvItem->IsSameItemAs(thisInvItem)) {
				// If same item type, stack and make currently held item the remainder
				int sum = prexistingInvItem->amount + thisInvItem->amount;
				int remainder = max(0, sum - prexistingInvItem->GetMaxStack());

				//onPlacedownCooldown = true;
				/*for(auto it = slotParentInventory->GetInventoryItems().begin(); it != slotParentInventory->GetInventoryItems().end(); ++it) {
					if(*it == prexistingInvItem) {
						
						break;
					}
				}*/
				if(slotParentInventory == invUI->GetInventory()) {
					invUI->FlagItemIconCooldown(prexistingInvItem);
				}

				prexistingInvItem->amount = min(sum, prexistingInvItem->GetMaxStack());
				if(remainder <= 0) {
					if(tableInterface)
					tableInterface->EraseIcon(this);
					invUI->EraseIcon(this);
					/*if(tableInterface) {
					}
					else {
					}*/


					for(auto it = inv->GetInventoryItems().begin(); it != inv->GetInventoryItems().end(); ++it) {
						if(*it == thisInvItem) {
							inv->GetInventoryItems().erase(it);
							break;
						}
					}

					delete thisInvItem;

					invUI->heldItem = nullptr;

					invUI->ReloadIcons();
					invUI->DrawHotbarIcons();
					if(tableInterface) tableInterface->ReloadIcons();

					delete this;
					return;
				}
				else {
					thisInvItem->amount = remainder;
				}
			}
			else { // if different types, swap
				InventoryItem::Type tempType = prexistingInvItem->type;
				unsigned short tempId = prexistingInvItem->ID;
				int tempAmount = prexistingInvItem->amount;

				prexistingInvItem->type = thisInvItem->type;
				prexistingInvItem->ID = thisInvItem->ID;
				prexistingInvItem->amount = thisInvItem->amount;

				thisInvItem->type = tempType;
				thisInvItem->ID = tempId;
				thisInvItem->amount = tempAmount;

				invUI->ReloadIcons();
				if(tableInterface) tableInterface->ReloadIcons();
			}
		}
	}
	else {// If no item exists here, simply put it down!
		thisInvItem->posX = idx;
		thisInvItem->posY = idy;

		SetParent(parentSlot);
		isHeld = false;

		AttemptStorageMigration(slotParentInventory, tableInterface);

		invUI->heldItem = nullptr;

		//invUI->ReloadIcons();
		//invUI->DrawHotbarIcons();
		if(tableInterface) tableInterface->ReloadIcons();
	}
	invUI->ReloadIcons();
	invUI->DrawHotbarIcons();
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

	if(onPlacedownCooldown) {
		onPlacedownCooldown = false;
		return;
	}

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

						//todo: encapsulate function
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
				newIcon->SetInventoryParent(inv);
				invUI->heldItem = newIcon;
				invUI->AddNewIcon(newIcon);
			}
			invUI->ReloadIcons();
		}

		Reload();
	}
}
