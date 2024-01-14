#include "DroppedItem.h"

#include "PlayerController.h"
#include "Inventory.h"
#include "PlayerData.h"

//Vector2 


void DroppedItem::SetupModel() {
	if(invItem != nullptr) {
		if(models.size() == 0) {
			AddModel(Graphics::Get()->GetDevice());
		}

		const string& atlasName = (invItem->type == InventoryItem::Type::BLOCK) ? "atlas" : "item-atlas";

		delete models[0]->GetMesh();
		models[0]->SetTexture(0, atlasName);

		Vector2Int uvPosInt = invItem->GetUVPos();
		Vector2 uvPos = { (float)uvPosInt.x, (float)uvPosInt.y };
		uvPos *= ATLAS_TILE_SIZE;
		uvPos /= (float)ATLAS_SIZE;

		Vector2 uvPosTop, uvPosBottom;
		uvPosTop = uvPos;
		uvPosBottom = uvPos;

		if(invItem->type == InventoryItem::Type::BLOCK) {
			uvPosInt = invItem->GetUVPos(BLOCK_FACE_TEX::FT_TOP);
			uvPosTop = { (float)uvPosInt.x, (float)uvPosInt.y};
			uvPosTop *= ATLAS_TILE_SIZE;
			uvPosTop /= (float)ATLAS_SIZE;

			uvPosInt = invItem->GetUVPos(BLOCK_FACE_TEX::FT_BOTTOM);
			uvPosBottom = { (float)uvPosInt.x, (float)uvPosInt.y };
			uvPosBottom *= ATLAS_TILE_SIZE;
			uvPosBottom /= (float)ATLAS_SIZE;
		}


		Mesh* newMesh = new Mesh();
		models[0]->SetMesh(newMesh);

		newMesh->_isProceduralMesh = true;
		newMesh->Init(Graphics::Get()->GetDevice());
		newMesh->LoadIndices(exampleCubeIndices, ARRAYSIZE(exampleCubeIndices));

		Vertex vertices[24];
		memcpy(vertices, exampleCubeVertices, sizeof(exampleCubeVertices));

		float tileSize = ATLAS_TILE_SIZE / (float)ATLAS_SIZE;

		for(int i = 0; i < 6*4; i+=4) {//for each face 
			vertices[0+i].uv = { 0.f, tileSize };
			vertices[1+i].uv = { 0.f, 0.f };
			vertices[2+i].uv = { tileSize, 0.f };
			vertices[3+i].uv = { tileSize, tileSize };

			switch(i) {
			case 16: // top
				vertices[0 + i].uvOffset = uvPosTop;
				vertices[1 + i].uvOffset = uvPosTop;
				vertices[2 + i].uvOffset = uvPosTop;
				vertices[3 + i].uvOffset = uvPosTop;
				break;
			case 20: // bottom
				vertices[0 + i].uvOffset = uvPosBottom;
				vertices[1 + i].uvOffset = uvPosBottom;
				vertices[2 + i].uvOffset = uvPosBottom;
				vertices[3 + i].uvOffset = uvPosBottom;
				break;
			default:
				vertices[0+i].uvOffset = uvPos;
				vertices[1+i].uvOffset = uvPos;
				vertices[2+i].uvOffset = uvPos;
				vertices[3+i].uvOffset = uvPos;
				break;
			}
		}

		newMesh->LoadVertices(vertices, ARRAYSIZE(vertices));


		//models[0]->SetMesh("cube");
		//models[0]->SetTexture(0, "head");
	}
}

void DroppedItem::OnCollide(PlayerController* pc) {
	if(invItem) {
		//Inventory* inv = Engine::Get()->GetCurrentScene()->GetObject3D<Inventory>("Inventory");
		PlayerData* playerData = Engine::Get()->GetCurrentScene()->GetObject3D<PlayerData>("PlayerData");
		Inventory* inv = playerData->GetInventory();
		
		int outRemainder = invItem->amount % invItem->GetMaxStack();

		if(inv->AddItem(invItem)) {
			// after its added, its merged with other inv items handled by inventory
			// delete this free instance
			delete invItem; 
			invItem = nullptr;
		}
		else {
			//// something to do with handling remainders
			invItem->amount = outRemainder;
		}
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

DroppedItem* DroppedItem::Create(InventoryItem* invItem, Vector3 worldPosition) {
	DroppedItem* dropped = new DroppedItem(invItem);
	dropped->transform.position = worldPosition;

	Engine* engine = Engine::Get();
	engine->GetCurrentScene()->CreateObject3D(dropped, "dropped-item-" + to_string(rand()) + to_string(rand()));

	return dropped;
}

void DroppedItem::Start() {
	Entity::Start();

	aabb = AABB(Vector3(0.f, 0.f, 0.f), Vector3(.25f, .25f, .25f));

	transform.scale = Vector3(0.5f, 0.5f, 0.5f);
	transform.rotation = Vector3(0.f, static_cast<float>(rand() % 360), 0.f);

	RecalculateGroundCheck();
}

void DroppedItem::Update(float dt) {
	Entity::Update(dt);

	transform.rotation.y += 2.f*dt;
}
