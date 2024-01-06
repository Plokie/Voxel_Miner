#include "PlayerController.h"

#include "VoxelRaycast.h"
#include "../Engine/UI/Label.h"
#include "ChunkManager.h"
#include "WorldGen.h"
#include "Chunk.h"
#include "InventoryUI.h"
#include "InventoryItem.h"
#include "Inventory.h"
#include "LootTables.h"
#include "Entity.h"
#include "DroppedItem.h"
//#include "ChunkDatabase.h"

#include "../Audio/Audio.h"

void PlayerController::Start()
{
	Engine* engine = Engine::Get();

	//transform.position = Vector3(0, 5, 0);

	engine->GetCurrentScene()->CreateObject3D(new Object3D(), "block_select", "cube", "block-select");
	this->blockSelectRef = engine->GetCurrentScene()->GetObject3D("block_select");
	blockSelectRef->models[0]->SetTransparent(true);
	blockSelectRef->transform.scale = Vector3(1.01f, 1.01f, 1.01f);

	this->fpsCounter = engine->GetCurrentScene()->GetObject2D<Label>("fps-counter");
	this->worldPosLabel = engine->GetCurrentScene()->GetObject2D<Label>("worldpos");

	this->inv = engine->GetCurrentScene()->GetObject3D<Inventory>("Inventory");

	//engine->GetCurrentScene()->CreateObject3D(new Object3D(), "a_debug_look", "cube", "err");
	//engine->GetCurrentScene()->GetObject3D("a_debug_look")->models[0]->SetTransparent(true);
	//engine->GetCurrentScene()->GetObject3D("a_debug_look")->transform.scale = Vector3(0.001f, 0.001f, 0.001f);

	//engine->GetCurrentScene()->CreateObject3D(new Object3D(), "cam_bounds", "inverse-cube", "block-select");
	//engine->GetCurrentScene()->GetObject3D("cam_bounds")->models[0]->SetTransparent(true);
	//engine->GetCurrentScene()->GetObject3D("cam_bounds")->transform.scale = Vector3(playerHalfExtents);

	aabb = AABB(transform.position, playerHalfExtents);

	chunkManager = engine->GetCurrentScene()->GetObject3D<ChunkManager>("AChunkManager");
}

#define AABB_RANGE Vector3Int(1,2,1)

vector<AABB> PlayerController::GetNearbyAABBs(ChunkManager* chunkManager) {
	vector<AABB> ret;

	for(int z =  - AABB_RANGE.z; z < AABB_RANGE.z + 1; z++) {
		for(int y =  - AABB_RANGE.y; y < AABB_RANGE.y + 1; y++) {
			for(int x =  - AABB_RANGE.x; x < AABB_RANGE.x + 1; x++) {
				Vector3 offset = Vector3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
				Vector3Int playerBlockPos = Vector3Int::FloorToInt(transform.position);
				Vector3Int blockPos = playerBlockPos + offset;
				BlockID block = chunkManager->GetBlockAtWorldPos(blockPos);
				if(BlockDef::GetDef(block).GetDrawType() == B_OPAQUE) {
					ret.push_back(AABB(Vector3(static_cast<float>(blockPos.x), static_cast<float>(blockPos.y), static_cast<float>(blockPos.z)) + Vector3(0.5f, 0.5f, 0.5f), Vector3(0.5f, 0.5f, 0.5f)));
				}
			}
		}
	}

	return ret;
}


void PlayerController::Update(float dTime)
{
	if(!enabled) {
		worldPosLabel->SetText(to_string(chunkManager->LoadMinAreaProgress()*100.f) + "%");

		if(chunkManager->HasLoadedMinArea()) {
			enabled = true;
			Input::SetMouseLocked(true);
		}
		
		return;
	}


	Engine* engine = Engine::Get();
	Camera* camera = &engine->GetGraphics()->camera;

	float movementSpeed = 4.317f;

	if(Input::IsMouseLocked())
	if(Input::IsKeyPressed('P')) {
		freeCam = !freeCam;
	}

	//todo: remove // debug spawn entity
	if(Input::IsMouseLocked())
	if(Input::IsKeyPressed('M')) {
		engine->GetCurrentScene()->CreateObject3D(new Entity(), "test-entity-"+to_string(rand()), "cube")->transform.position = transform.position;
		//DroppedItem::Create(new InventoryItem(COBBLESTONE, -1, -1, 1), transform.position + (transform.forward() * 1.46f));
	}

	if(Input::IsKeyPressed('Q')) {
		//engine->GetCurrentScene()->CreateObject3D(new Entity(), "test-entity-"+to_string(rand()), "cube")->transform.position = transform.position;
		InventoryItem* heldItem;
		if(inv->GetHeldItem(&heldItem)) {

			InventoryItem* newDroppedItem = new InventoryItem(heldItem->type, heldItem->ID, -1, -1, 1);
			
			DroppedItem* droppedItemEntity = DroppedItem::Create(newDroppedItem, transform.position + (transform.forward() * 1.46f));
			//droppedItemEntity->SetVelocity(transform.forward());

			inv->SubHeldItem(1, heldItem); 
		}
		
	}

	if(Input::IsMouseLocked()) {
		bool isSprinting = false;
		if(Input::IsKeyHeld(VK_SHIFT)) {
			//transform.position -= Vector3(0, camSpeed, 0);
			movementSpeed = 5.612f;
			isSprinting = true;
		}
		if(Input::IsKeyHeld(VK_CONTROL)) {
			movementSpeed = 1.0f;
		}
		if(freeCam) {
			movementSpeed = 10.0f;
		}

		Vector2 input = Input::GetInputVector().normalized();
		Vector3 moveAxis = transform.basis(input.x, 0, input.y);
		moveAxis.y = 0.0;
		moveAxis = moveAxis.normalized();
		moveAxis *= movementSpeed * dTime;

		if(input.sqrMagnitude() > 0.5f) {
			inv->SetHungerFlag(HDS_WALK, true);
			inv->SetHungerFlag(HDS_SPRINT, isSprinting);
		}
		else {
			inv->SetHungerFlag(HDS_WALK, false);
			inv->SetHungerFlag(HDS_SPRINT, false);
		}
		//velocity.x = moveAxis.x;
		//velocity.z = moveAxis.z;

		if(Input::IsPadButtonHeld(0, XINPUT_GAMEPAD_A)) {
			transform.position += Vector3(0, movementSpeed, 0);
		}

		XMFLOAT2 mouseDelta = Input::MouseDelta();
		float lookSpeed = 0.0025f; // The polling of the mouse is already tied to the framerate, so no dt


		transform.position += moveAxis;
		transform.rotation += Vector3(mouseDelta.y * lookSpeed, mouseDelta.x * lookSpeed, 0.f);
	}

	if(Input::IsKeyPressed(VK_ESCAPE)) {
		Input::SetMouseLocked(!Input::IsMouseLocked());
	}


	if(!freeCam) {
		// GRAVITY PHYSICS
		vector<AABB> blocks = GetNearbyAABBs(chunkManager);


		bool isGrounded = false;
		for(const AABB& blockAABB : blocks) {
			for(const Vector3& v : groundCheckPoints) {
				if(blockAABB.IsPointWithin(transform.position - v)) {
					isGrounded = true;
					break;
				}
			}
		}

		if(!isGrounded) {
			velocity.y += gravity * dTime;
			velocity.y = max(velocity.y, terminalVelocity);
		}
		else {
			velocity.y = -3.0f * dTime; //Small nudge to ground level, nothing noticable
			if(Input::IsKeyHeld(VK_SPACE) && Input::IsMouseLocked()) 
			{
				inv->DecrementHungerFlag(HDS_JUMP);
				velocity.y = jumpVelocity;
			}
		}

		transform.position += velocity * dTime;


		// AABB COLLISION CHECK
		aabb.SetPosition(transform.position - Vector3(0, 0.62f, 0));

		for(const AABB& blockAABB : blocks) {
			if(aabb.Intersects(blockAABB)) {
				transform.position += AABB::penetration_vector(AABB::minkowski_difference(blockAABB, aabb));
				aabb.SetPosition(transform.position - Vector3(0, 0.62f, 0));
			}
		}
	}
	else {
		if(Input::IsKeyHeld(VK_SPACE) && Input::IsMouseLocked()) {
			transform.position.y += movementSpeed * dTime;
		}
		if(Input::IsKeyHeld(VK_SHIFT) && Input::IsMouseLocked()) {
			transform.position.y -= movementSpeed * dTime;
		}
	}

	Vector3Int playerBottomPos = Vector3Int::FloorToInt(transform.position - Vector3(0, playerHalfExtents.y, 0));
	Vector3Int playerTopPos = Vector3Int::FloorToInt(transform.position);


	// TEMP TODO:REMOVE AND PUT SOMEWHERE MORE RELEVANT
	if(Input::IsKeyPressed('1')) inv->SetSlotNum(0);
	if(Input::IsKeyPressed('2')) inv->SetSlotNum(1);
	if(Input::IsKeyPressed('3')) inv->SetSlotNum(2);
	if(Input::IsKeyPressed('4')) inv->SetSlotNum(3);
	if(Input::IsKeyPressed('5')) inv->SetSlotNum(4);
	if(Input::IsKeyPressed('6')) inv->SetSlotNum(5);
	if(Input::IsKeyPressed('7')) inv->SetSlotNum(6);
	if(Input::IsKeyPressed('8')) inv->SetSlotNum(7);
	if(Input::IsKeyPressed('9')) inv->SetSlotNum(8);
	if(Input::IsKeyPressed('0')) inv->SetSlotNum(9);

	if(Input::GetMouseScrollDelta() > 0) {
		inv->ChangeSlotNum(-1);
	}
	else if(Input::GetMouseScrollDelta() < 0) {
		inv->ChangeSlotNum(1);
	}

	// PLAYER RAYCAST
	VoxelRay ray(transform.position, transform.forward());

	Vector3Int lookHitPoint = Vector3Int(0, 0, 0);
	Vector3Int lookHitNormal;
	BlockID lookHitBlock;
	if(VoxelRay::Cast(&ray, chunkManager, 10.f, &lookHitPoint, &lookHitBlock, &lookHitNormal)) {
		blockSelectRef->models[0]->alpha = 0.99f; // fails sometimes
		blockSelectRef->transform.position = Vector3((float)lookHitPoint.x, (float)lookHitPoint.y, (float)lookHitPoint.z) + Vector3(0.5f, 0.5f, 0.5f);


		// INPUT MODIFY
		if(Input::IsMouseLocked()) {
			if(Input::IsMouseKeyPressed(MOUSE_L)) {
				InventoryItem* invItem;
				
				if(inv->GetHeldItem(&invItem)) {
					BlockID targetBlock = chunkManager->GetBlockAtWorldPos(lookHitPoint);
					Block blockDef = BlockDef::GetDef(targetBlock);
					Item itemDef = (invItem->type == InventoryItem::Type::BLOCK)?ItemDef::Get((ItemID)0):ItemDef::Get((ItemID)invItem->ID);

					if((itemDef.GetItemType() == blockDef.GetMineType() && itemDef.GetTier() >= blockDef.GetTier() ) || blockDef.GetMineType() == BASICITEM) {
						Audio::Play("hit", 1.f);
						chunkManager->SetBlockAtWorldPos(lookHitPoint, AIR);
						
						inv->ChangeScore(blockDef.GetTier());

						if(blockDef.GetLootTableName() != "") {
							const InventoryItem loot = LootTable::Choose(blockDef.GetLootTableName());


							//inv->AddItem(loot.ID, loot.type, loot.amount);

							InventoryItem* newDroppedItem = new InventoryItem(loot.type, loot.ID, -1, -1, loot.amount);
							DroppedItem::Create(newDroppedItem, lookHitPoint);
						}
						else 
						{
							//inv->AddItem(targetBlock);

							InventoryItem* newDroppedItem = new InventoryItem(targetBlock, -1, -1, 1);
							DroppedItem::Create(newDroppedItem, lookHitPoint);
						}

					}

				}

			}

			if(Input::IsMouseKeyPressed(MOUSE_R)) {
				//inv->GetItemAt()

				if(Block::CallBlockAction(lookHitBlock, this, inv, chunkManager, lookHitPoint)) {

				}
				else {
					InventoryItem* invItem;
					if(inv->GetHeldItem(&invItem) && invItem->type==InventoryItem::Type::BLOCK) {

						AABB targetBlockAABB = AABB(lookHitPoint + lookHitNormal + Vector3(0.5f, 0.5f, 0.5f), Vector3(0.5f, 0.5f, 0.5f));
						if(!targetBlockAABB.Intersects(aabb)) {
							chunkManager->SetBlockAtWorldPos(lookHitPoint + lookHitNormal, (BlockID)invItem->ID);
							//inv->SubItem((BlockID)invItem->ID);
							/*invItem->amount -= 1;
							if(invItem->amount<=0) inv->ClearEmptyItems();*/
							inv->SubHeldItem(1, invItem);
						}
					}
				}



			}
		}
	}
	else {
		blockSelectRef->models[0]->alpha = 0.0f;
	}


	// DEBUG INFO
	fpsCounter->SetText(to_string(static_cast<int>(roundf(1.f / dTime))));

	Vector3Int camBlockPos = Vector3Int::FloorToInt(transform.position);
	Vector3Int footPos = camBlockPos - Vector3Int(0, 1, 0);
	Vector3Int chunkIndex = ChunkManager::ToChunkIndexPositionTuple(footPos.x, footPos.y, footPos.z);

	float temp = WorldGen::SampleTemperature(footPos.x, footPos.z);
	float moist = WorldGen::SampleMoisture(footPos.x, footPos.z);

	worldPosLabel->SetText(
		footPos.ToString() + "\n"
		+ chunkIndex.ToString() + "\n"
		+ Vector3Int(FloorMod(camBlockPos.x, CHUNKSIZE_X), FloorMod(camBlockPos.y, CHUNKSIZE_Y), FloorMod(camBlockPos.z, CHUNKSIZE_Z)).ToString() + "\n"
		+ to_string(chunkManager->GetBlockLightAtWorldPos(footPos)) + "\n"
		+ to_string(chunkManager->GetSkyLightAtWorldPos(footPos)) + "\n"
		+ "T: " + to_string(temp) + "\n"
		+ "M: " + to_string(moist) + "\n"
		+ "B: " + Biome::Get(temp, moist).name + "\n"
	);

	// KEEP AT END
	camera->transform = transform;
}

