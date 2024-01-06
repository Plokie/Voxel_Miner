#include "HeldItem.h"

#include "../Engine/Engine.h"
#include "../Graphics/Graphics.h"

#include "Meshes/HeldItemMesh.h"

HeldItem::HeldItem() {
}

void HeldItem::Start() {
	//AddModel()
	AddModel(Graphics::Get()->GetDevice());
	//models[0]->SetMesh("heldItemMesh");
	SetItem(ItemID::ITEMERR); //blanks held item
}

void HeldItem::SetItem(BlockID blockID) {
	SetItem(InventoryItem::Type::BLOCK, blockID);
}

void HeldItem::SetItem(ItemID itemID) {
	SetItem(InventoryItem::Type::ITEM, itemID);
}

void HeldItem::SetItem(InventoryItem::Type type, unsigned short id) {
	const string& atlasName = (type == InventoryItem::Type::BLOCK) ? "atlas" : "item-atlas";

	if(models.size() == 0) return;

	delete models[0]->GetMesh();
	models[0]->SetTexture(0, atlasName);
	

	if(id == 0) models[0]->alpha = 0.0f;
	else models[0]->SetTransparent(true);

	Vector2Int uvPosInt = InventoryItem(type, id, -1, -1, 1).GetUVPos();
	Vector2 uvPos = { (float)uvPosInt.x, (float)uvPosInt.y };
	uvPos *= ATLAS_TILE_SIZE;
	uvPos /= (float)ATLAS_SIZE;


	Mesh* newMesh = new Mesh();

	models[0]->SetMesh(newMesh);

	newMesh->_isProceduralMesh = true;
	newMesh->Init(Graphics::Get()->GetDevice());
	newMesh->LoadIndices(heldItemIndices, ARRAYSIZE(heldItemIndices));

	Vertex vertices[4];
	memcpy(vertices, heldItemVertices, sizeof(heldItemVertices));

	float tileSize = ATLAS_TILE_SIZE / (float)ATLAS_SIZE;

	vertices[0].uv = { tileSize, tileSize };
	vertices[1].uv = { tileSize, 0.f };
	vertices[2].uv = { 0.f, 0.f };
	vertices[3].uv = { 0.f, tileSize };

	vertices[0].uvOffset = uvPos;
	vertices[1].uvOffset = uvPos;
	vertices[2].uvOffset = uvPos;
	vertices[3].uvOffset = uvPos;

	newMesh->LoadVertices(vertices, ARRAYSIZE(vertices));

	//models[0]->SetMesh(;
	//models[0]

	//icon = new UIRect(atlasName, { 1.f,1.f,1.f,1.f });
	//icon->Init(Graphics::Get()->GetDevice());
	//icon->SetDimensions({ 50.f,50.f });
	//icon->SetAnchor({ .5f,.5f });
	//icon->SetPivot({ .5f,.5f });
	//icon->SetDepth(13.f);
	//icon->SetParent(this);


	//LONG texPosX = static_cast<LONG>(ITEM_ATLAS_TILE_SIZE * uvPos.x);
	//LONG texPosY = static_cast<LONG>(ITEM_ATLAS_TILE_SIZE * uvPos.y);

	//icon->SetTexRect({
	//	texPosX, texPosY,
	//	texPosX + ITEM_ATLAS_TILE_SIZE,
	//	texPosY + ITEM_ATLAS_TILE_SIZE
	//	});

}
