#include "PlayerController.h"

#include "VoxelRaycast.h"
#include "../Engine/UI/Label.h"
#include "ChunkManager.h"
#include "WorldGen.h"
#include "Chunk.h"
#include "InventoryUI.h"
#include "InventoryItem.h"
#include "Inventory.h"
#include "PlayerData.h"
#include "LootTables.h"
#include "Entity.h"
#include "DroppedItem.h"
#include "BlockAction.h"
//#include "ChunkDatabase.h"

#include "../Audio/Audio.h"
#include "PauseScreen.h"

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

	//this->inv = engine->GetCurrentScene()->GetObject3D<Inventory>("Inventory");
	this->_pCurrentPlayerData = engine->GetCurrentScene()->GetObject3D<PlayerData>("PlayerData");

	_pCurrentPlayerData->AddOnDeathEvent([this, engine]() {
		_pCurrentPlayerData->GetInventory()->DropAllItems(transform.position);
		transform.position = { 0.f, ceil(WorldGen::SampleWorldHeight(0, 0)) + 3.f, 0.f};
		
		Camera* camera = &engine->GetGraphics()->camera;
		camera->transform = transform;

		aabb.SetPosition(transform.position - Vector3(0, 0.62f, 0));

		_pCurrentPlayerData->GetInventory()->AddItem(COPPER_PICKAXE);
		_pCurrentPlayerData->GetInventory()->AddItem(COPPER_AXE);
		_pCurrentPlayerData->GetInventory()->AddItem(COPPER_SHOVEL);
		_pCurrentPlayerData->GetInventory()->AddItem(WORKBENCH, 5);
		_pCurrentPlayerData->GetInventory()->AddItem(APPLE, 5);

		InventoryUI* invUI = engine->GetCurrentScene()->GetObject2D<InventoryUI>("invUI");
		invUI->ReloadIcons();
		invUI->DrawHotbarIcons();
	});

	Input::SetVirtualMouse(false);

	insideBlock = engine->GetCurrentScene()->CreateObject3D(new Object3D(), "insideBlock", "inverse-cube", "atlas");
	insideBlock->models[0]->SetTransparent(true);
	insideBlock->transform.position = Vector3(0.f, 0.f, 0.f);
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

vector<AABB> PlayerController::GetNearbyAABBs(ChunkManager* chunkManager, vector<AABB>* outDamageAABBs, vector<AABB>* outLiquidAABBs) {
	vector<AABB> ret;
	//vector<AABB> statusAABBs; //for now only used for damaging

	for(int z =  - AABB_RANGE.z; z < AABB_RANGE.z + 1; z++) {
		for(int y =  - AABB_RANGE.y; y < AABB_RANGE.y + 1; y++) {
			for(int x =  - AABB_RANGE.x; x < AABB_RANGE.x + 1; x++) {
				Vector3 offset = Vector3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
				Vector3Int playerBlockPos = Vector3Int::FloorToInt(transform.position);
				Vector3Int blockPos = playerBlockPos + offset;
				BlockID block = chunkManager->GetBlockAtWorldPos(blockPos);
				AABB blockAABB = AABB(Vector3(static_cast<float>(blockPos.x), static_cast<float>(blockPos.y), static_cast<float>(blockPos.z)) + Vector3(0.5f, 0.5f, 0.5f), Vector3(0.5f, 0.5f, 0.5f));

				if(BlockDef::GetDef(block).IsSolid()) {
					ret.push_back(blockAABB);
				}

				if(outDamageAABBs && block == LAVA) {
					outDamageAABBs->push_back(blockAABB);
				}

				if(outLiquidAABBs && (block == WATER || block == LAVA)) {
					outLiquidAABBs->push_back(blockAABB);
				}
			}
		}
	}

	return ret;
}

void PlayerController::Update(float dTime)
{
	bool didDoubleTapJump = false;
	timeSinceLastJump += dTime;
	if(Input::IsKeyPressed(VK_SPACE) || Input::IsPadButtonPressed(0, XINPUT_GAMEPAD_A)) {
		
		if(timeSinceLastJump <= .3f) {
			didDoubleTapJump = true;
		}
		else {
			timeSinceLastJump = 0.f;
		}
	}

	Inventory* inv = _pCurrentPlayerData->GetInventory();

	if(!enabled) {
		worldPosLabel->SetText(to_string(chunkManager->LoadMinAreaProgress()*100.f) + "%");
		fpsCounter->SetText("Loading...");
		if(chunkManager->HasLoadedMinArea()) {
			enabled = true;
			Input::SetMouseLocked(true);
		}
		
		return;
	}


	Engine* engine = Engine::Get();
	Camera* camera = &engine->GetGraphics()->camera;

	float movementSpeed = 4.317f;

#ifdef _DEBUG
	if(Input::IsMouseLocked())
	if(Input::IsKeyPressed('P') || Input::IsPadButtonPressed(0, XINPUT_GAMEPAD_DPAD_LEFT)) {
		freeCam = !freeCam;
	}

	//todo: remove // debug spawn entity
	if(Input::IsMouseLocked())
	if(Input::IsKeyPressed('M') || Input::IsPadButtonPressed(0, XINPUT_GAMEPAD_DPAD_RIGHT)) {
		engine->GetCurrentScene()->CreateObject3D(new Entity(), "test-entity-"+to_string(rand()), "cube")->transform.position = transform.position;
		//DroppedItem::Create(new InventoryItem(COBBLESTONE, -1, -1, 1), transform.position + (transform.forward() * 1.46f));
	}
#endif

	if(Input::IsKeyPressed('Q') || Input::IsPadButtonPressed(0, XINPUT_GAMEPAD_X)) {
		//engine->GetCurrentScene()->CreateObject3D(new Entity(), "test-entity-"+to_string(rand()), "cube")->transform.position = transform.position;
		InventoryItem* heldItem;
		if(inv->GetHeldItem(&heldItem)) {

			//declare before possible goto call
			InventoryItem* newDroppedItem;
			DroppedItem* droppedItemEntity;

			if(!_pCurrentPlayerData->IsCreative())
			if(heldItem->type == InventoryItem::ITEM  ) {
				Item itemDef = ItemDef::Get(static_cast<ItemID>(heldItem->ID));
				if(itemDef.GetItemType() != BASICITEM && itemDef.GetItemType() != FOOD) { //todo: special item type check

					if(inv->GetToolsOfType(itemDef.GetItemType()).size() <= 1) {
						goto passdrop; // skip item drop logic since the player only has one tool of this type
						// we dont want the player to drop their only pickaxe for example
					}

				}
			}

			newDroppedItem = new InventoryItem(heldItem->type, heldItem->ID, -1, -1, 1);
			droppedItemEntity = DroppedItem::Create(newDroppedItem, transform.position + (transform.forward() * 1.46f));
			inv->SubHeldItem(1, heldItem); 

		passdrop: __nop();
		}
		
	}

	if(Input::IsMouseLocked()) {
		bool isSprinting = false;
		if(Input::IsKeyHeld(VK_SHIFT) || Input::IsPadButtonHeld(0, XINPUT_GAMEPAD_LEFT_THUMB)) {
			//transform.position -= Vector3(0, camSpeed, 0);
			movementSpeed = 5.612f;
			isSprinting = true;
		}
		if(Input::IsKeyHeld(VK_CONTROL) || Input::IsPadButtonHeld(0, XINPUT_GAMEPAD_RIGHT_THUMB)) {
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
			_pCurrentPlayerData->SetHungerFlag(HDS_WALK, true);
			_pCurrentPlayerData->SetHungerFlag(HDS_SPRINT, isSprinting);
		}
		else {
			_pCurrentPlayerData->SetHungerFlag(HDS_WALK, false);
			_pCurrentPlayerData->SetHungerFlag(HDS_SPRINT, false);
		}

		XMFLOAT2 mouseDelta = Input::MouseDelta();
		float mouseSens = 0.0025f; // The polling of the mouse is already tied to the framerate, so no dt
		//float joystickSens = 0.007f;
		float joystickSens = dTime * 3.75f;

		Vector2 lookInput = Input::RightAxis(0) * joystickSens;
		lookInput.y *= -1.f;

		lookInput.x += mouseDelta.x * mouseSens;
		lookInput.y += mouseDelta.y * mouseSens;



		transform.position += moveAxis;
		transform.rotation += Vector3(lookInput.y, lookInput.x, 0.f);
	}

	if((Input::IsKeyPressed(VK_ESCAPE) || Input::IsPadButtonPressed(0, XINPUT_GAMEPAD_START)) && Input::IsMouseLocked()) {
		Input::SetMouseLocked(!Input::IsMouseLocked());

		if(!Input::IsMouseLocked()) {
			// Open pause screen
			PauseScreen::Open();
		}
		else {
			// Close pause screen
			PauseScreen::Close();
		}
	}

	if(didDoubleTapJump && _pCurrentPlayerData->IsCreative()) {
		freeCam = !freeCam;
	}

	if(!freeCam) {
		// GRAVITY PHYSICS
		vector<AABB> damageAABBs;
		vector<AABB> liquidAABBs;
		vector<AABB> blocks = GetNearbyAABBs(chunkManager, &damageAABBs, &liquidAABBs);

		bool takingDamage = false;
		for(const AABB& damageAABB : damageAABBs) {
			if(aabb.Intersects(damageAABB)) {
				takingDamage = true;
				break;
			}
		}
		_pCurrentPlayerData->SetDamageFlag(DS_LAVA, takingDamage);
		
		bool inLiquid = false;
		for(const AABB& liquidAABB : liquidAABBs) {
			if(aabb.Intersects(liquidAABB)) {
				inLiquid = true;
				break;
			}
		}

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
			velocity.y += gravity * dTime * ((inLiquid)?0.2f:1.0f);
			// transition between terminal velocities depending on inLiquid
			currentTerminalVelocity = lerp(currentTerminalVelocity, inLiquid ? liquidTerminalVelocity : terminalVelocity, dTime * 13.5f);
			velocity.y = max(velocity.y, currentTerminalVelocity);
		}
		else {
			if(velocity.y < -20.f && !_pCurrentPlayerData->IsCreative()) {
				// fall damage
				_pCurrentPlayerData->ChangeHealth(static_cast<int>((velocity.y * 1.5f) / 5.f));
			}

			velocity.y = -3.0f * dTime; //Small nudge to ground level, nothing noticable
			if(!inLiquid) {
				if((Input::IsKeyHeld(VK_SPACE) || Input::IsPadButtonHeld(0, XINPUT_GAMEPAD_A)) && Input::IsMouseLocked())
				{
					_pCurrentPlayerData->DecrementHungerFlag(HDS_JUMP);
					velocity.y = jumpVelocity;
				}
			}
		}

		if(inLiquid) {
			if((Input::IsKeyHeld(VK_SPACE) || Input::IsPadButtonHeld(0, XINPUT_GAMEPAD_A)) && Input::IsMouseLocked())
			{
				velocity.y = 3.0f;
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
		if((Input::IsKeyHeld(VK_SPACE) || Input::IsPadButtonHeld(0, XINPUT_GAMEPAD_A)) && Input::IsMouseLocked()) {
			transform.position.y += movementSpeed * dTime;
		}
		if((Input::IsKeyHeld(VK_SHIFT) || Input::IsPadButtonHeld(0, XINPUT_GAMEPAD_RIGHT_THUMB)) && Input::IsMouseLocked()) {
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

	if(Input::GetMouseScrollDelta() > 0 || Input::IsPadButtonPressed(0, XINPUT_GAMEPAD_LEFT_SHOULDER)) {
		inv->ChangeSlotNum(-1);
	}
	else if(Input::GetMouseScrollDelta() < 0 || Input::IsPadButtonPressed(0, XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
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
			if(Input::IsMouseKeyPressed(MOUSE_L) || Input::IsRightTriggerPressed(0)) { // Mine
				InventoryItem* invItem;
				BlockID targetBlock = chunkManager->GetBlockAtWorldPos(lookHitPoint);
				Block blockDef = BlockDef::GetDef(targetBlock);
				
				Item itemDef = ItemDef::Get(COAL);
				if(inv->GetHeldItem(&invItem)) {
					itemDef = (invItem->type == InventoryItem::Type::BLOCK)?ItemDef::Get((ItemID)0):ItemDef::Get((ItemID)invItem->ID);
				}

				if(_pCurrentPlayerData->IsCreative() || blockDef.GetMineType() == BASICITEM || (itemDef.GetItemType() == blockDef.GetMineType() && itemDef.GetTier() >= blockDef.GetTier() )) {
					Audio::Play("hit", 1.f);
					chunkManager->SetBlockAtWorldPos(lookHitPoint, AIR);
						
					_pCurrentPlayerData->ChangeScore(blockDef.GetTier());

					if(blockDef.GetLootTableName() != "") {
						const InventoryItem loot = LootTable::Choose(blockDef.GetLootTableName());


						//inv->AddItem(loot.ID, loot.type, loot.amount);

						InventoryItem* newDroppedItem = new InventoryItem(loot.type, loot.ID, -1, -1, loot.amount);

						if(!_pCurrentPlayerData->IsCreative())
						DroppedItem::Create(newDroppedItem, lookHitPoint);
					}
					else 
					{
						//inv->AddItem(targetBlock);

						InventoryItem* newDroppedItem = new InventoryItem(targetBlock, -1, -1, 1);

						if(!_pCurrentPlayerData->IsCreative())
						DroppedItem::Create(newDroppedItem, lookHitPoint);
					}

				}


			}

			if(Input::IsMouseKeyPressed(MOUSE_R) || Input::IsLeftTriggerPressed(0)) {
				//inv->GetItemAt()

				if(BlockAction::CallBlockAction(lookHitBlock, this, _pCurrentPlayerData, chunkManager, lookHitPoint)) {

				}
				else {
					InventoryItem* invItem;
					if(inv->GetHeldItem(&invItem) && invItem->type == InventoryItem::Type::BLOCK) {
						AABB targetBlockAABB = AABB(lookHitPoint + lookHitNormal + Vector3(0.5f, 0.5f, 0.5f), Vector3(0.5f, 0.5f, 0.5f));
						if(!targetBlockAABB.Intersects(aabb)) {
							chunkManager->SetBlockAtWorldPos(lookHitPoint + lookHitNormal, (BlockID)invItem->ID);
							//inv->SubItem((BlockID)invItem->ID);
							/*invItem->amount -= 1;
							if(invItem->amount<=0) inv->ClearEmptyItems();*/
							if(!_pCurrentPlayerData->IsCreative())
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

	if(Input::IsMouseKeyPressed(MOUSE_R) || Input::IsLeftTriggerPressed(0)) {
		InventoryItem* invItem; // should get it again here since actions are called before this. Item in hand COULD disappear
		if(inv->GetHeldItem(&invItem) && invItem->type == InventoryItem::ITEM) {
			Item::CallItemAction((ItemID)invItem->ID, this, _pCurrentPlayerData, chunkManager, lookHitPoint);
		}
	}

	// DEBUG INFO
	fpsCounter->SetText(to_string(static_cast<int>(roundf(1.f / dTime))));
	

//#ifdef _DEBUG
	//Vector3Int camBlockPos = Vector3Int::FloorToInt(transform.position);
	//Vector3Int footPos = camBlockPos - Vector3Int(0, 1, 0);
	//Vector3Int chunkIndex = ChunkManager::ToChunkIndexPositionTuple(footPos.x, footPos.y, footPos.z);

	//float temp = WorldGen::SampleTemperature(footPos.x, footPos.z);
	//float moist = WorldGen::SampleMoisture(footPos.x, footPos.z);


	// this debug info freezes release builds often
	// 
	//worldPosLabel->SetText(
	//	footPos.ToString() + "\n"
	//	+ chunkIndex.ToString() + "\n"
	//	+ Vector3Int(FloorMod(camBlockPos.x, CHUNKSIZE_X), FloorMod(camBlockPos.y, CHUNKSIZE_Y), FloorMod(camBlockPos.z, CHUNKSIZE_Z)).ToString() + "\n"
	//	+ to_string(chunkManager->GetBlockLightAtWorldPos(footPos)) + "\n"
	//	+ to_string(chunkManager->GetSkyLightAtWorldPos(footPos)) + "\n"
	//	+ "T: " + to_string(temp) + "\n"
	//	+ "M: " + to_string(moist) + "\n"
	//	+ "B: " + Biome::Get(temp, moist).name + "\n"
	//);
//#else
	Vector3Int camBlockPos = Vector3Int::FloorToInt(transform.position);
	Vector3Int footPos = camBlockPos - Vector3Int(0, 1, 0);
	worldPosLabel->SetText(footPos.ToString());
//#endif

	// KEEP AT END
	camera->transform = transform;
	insideBlock->transform = transform;

	BlockID currentHeadBlockID = chunkManager->GetBlockAtWorldPos(camBlockPos);
	Block currentHeadBlock = BlockDef::GetDef(currentHeadBlockID);
	if(currentHeadBlockID==AIR || (freeCam && currentHeadBlock.IsSolid())) {
		insideBlock->doRender = false;
	}
	else {
		insideBlock->doRender = true;
		Vector2 uv = currentHeadBlock.GetBottomUV();
		insideBlock->models[0]->SetUVOffset(uv);
		insideBlock->transform.scale = Vector3(0.25f, 0.25f, 0.25f);
		insideBlock->transform.rotation.z += 0.01f;
	}

}

PlayerController::~PlayerController()
{
	Object3D::~Object3D();

	Transform& camTransform = Graphics::Get()->camera.transform;
	camTransform.position = Vector3(0.f, 0.f, 0.f);
	camTransform.rotation = Vector3(0.f, 0.f, 0.f);
}

