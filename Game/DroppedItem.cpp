#include "DroppedItem.h"

#include "PlayerController.h"
#include "Inventory.h"

void DroppedItem::SetupModel()
{
	if(invItem != nullptr) {
		if(models.size() == 0) {
			AddModel(Graphics::Get()->GetDevice());
		}


		models[0]->SetMesh("cube");
		models[0]->SetTexture(0, "head");
	}
}

void DroppedItem::OnCollide(PlayerController* pc) {
	if(invItem) {
		Engine::Get()->GetCurrentScene()->GetObject3D<Inventory>("Inventory")->PushItem(invItem);
	}

	Engine::Get()->DestroyObject3D(this);
}

void DroppedItem::Annihilate() {
	if(invItem) {
		delete invItem;
		invItem = nullptr;
	}
	//delete this;
	Engine::Get()->DestroyObject3D(this);
}

DroppedItem::DroppedItem(InventoryItem* invItem) {
	this->invItem = invItem;
	SetupModel();
}

void DroppedItem::Start()
{
	Entity::Start();

	aabb = AABB(Vector3(0.f, 0.f, 0.f), Vector3(.25f, .25f, .25f));

	transform.scale = Vector3(0.5f, 0.5f, 0.5f);

	RecalculateGroundCheck();
}

void DroppedItem::Update(float dt)
{
	Entity::Update(dt);
}
