#include "PlayerController.h"

#include "VoxelRaycast.h"
#include "../Engine/Label.h"
#include "ChunkManager.h"
#include "ChunkDatabase.h"

void CameraController::Start()
{
	Engine* engine = Engine::Get();

	transform.position = Vector3(0, 5, 0);

	engine->GetCurrentScene()->CreateObject3D(new Object3D(), "a_debug_pos", "cube", "block-select");
	engine->GetCurrentScene()->GetObject3D("a_debug_pos")->models[0]->SetTransparent(true);
	engine->GetCurrentScene()->GetObject3D("a_debug_pos")->transform.scale = Vector3(0.51f, 0.51f, 0.51f);

	engine->GetCurrentScene()->CreateObject3D(new Object3D(), "a_debug_look", "cube", "err");
	engine->GetCurrentScene()->GetObject3D("a_debug_look")->models[0]->SetTransparent(true);
	engine->GetCurrentScene()->GetObject3D("a_debug_look")->transform.scale = Vector3(0.001f, 0.001f, 0.001f);

	//engine->GetCurrentScene()->CreateObject3D(new Object3D(), "cam_bounds", "inverse-cube", "block-select");
	//engine->GetCurrentScene()->GetObject3D("cam_bounds")->models[0]->SetTransparent(true);
	//engine->GetCurrentScene()->GetObject3D("cam_bounds")->transform.scale = Vector3(playerHalfExtents);

	aabb = AABB(transform.position, playerHalfExtents);

}

#define AABB_RANGE Vector3Int(2,4,2)

vector<AABB> CameraController::GetNearbyAABBs(ChunkManager* chunkManager) {
	vector<AABB> ret;

	for(int z = 1-AABB_RANGE.z; z < AABB_RANGE.z+1; z++) {
		for(int y = 1-AABB_RANGE.y; y < AABB_RANGE.y+1; y++) {
			for(int x = 1-AABB_RANGE.x; x < AABB_RANGE.x+1; x++) {
				Vector3 offset = Vector3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
				Vector3Int playerBlockPos = Vector3Int::FloorToInt(transform.position);
				Vector3Int blockPos = playerBlockPos + offset;
				BlockID block = chunkManager->GetBlockAtWorldPos(blockPos);
				if(BlockDef::GetDef(block).IsSolid()) {
					ret.push_back(AABB(Vector3(static_cast<float>(blockPos.x), static_cast<float>(blockPos.y), static_cast<float>(blockPos.z)) + Vector3(0.5f, 0.5f, 0.5f), Vector3(0.5f, 0.5f, 0.5f)));
				}
			}
		}
	}

	return ret;
}


void CameraController::Update(float dTime)
{
	Engine* engine = Engine::Get();
	Camera* camera = &engine->GetGraphics()->camera;
	ChunkManager* chunkManager = engine->GetCurrentScene()->GetObject3D<ChunkManager>("ChunkManager");

	if(Input::IsMouseLocked()) {
		float movementSpeed = 4.317f;
		if (Input::IsKeyHeld(VK_SHIFT)) {
			//transform.position -= Vector3(0, camSpeed, 0);
			movementSpeed = 5.612f;
		}
		if(Input::IsKeyHeld(VK_CONTROL)) {
			movementSpeed = 1.0f;
		}

		Vector2 input = Input::GetInputVector().normalized();
		Vector3 moveAxis = transform.basis(input.x, 0, input.y);
		moveAxis.y = 0.0;
		moveAxis = moveAxis.normalized();
		moveAxis *= movementSpeed * dTime;

		transform.position += moveAxis;
		//velocity.x = moveAxis.x;
		//velocity.z = moveAxis.z;
	

		if (Input::IsPadButtonHeld(0, XINPUT_GAMEPAD_A)) {
			transform.position += Vector3(0, movementSpeed, 0);
		}

		XMFLOAT2 mouseDelta = Input::MouseDelta();
		float lookSpeed = 0.0025f; // The polling of the mouse is already tied to the framerate, so no dt
	


		transform.rotation += Vector3(mouseDelta.y * lookSpeed, mouseDelta.x * lookSpeed, 0.f);
	}

	if (Input::IsKeyPressed(VK_ESCAPE)) {
		Input::SetMouseLocked(!Input::IsMouseLocked());
	}

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
		velocity.y = -1.0f * dTime; //Small nudge to ground level, nothing noticable
		if(Input::IsKeyHeld(VK_SPACE)) velocity.y = jumpVelocity;
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

	Vector3Int playerBottomPos = Vector3Int::FloorToInt(transform.position - Vector3(0, playerHalfExtents.y, 0));
	Vector3Int playerTopPos = Vector3Int::FloorToInt(transform.position);


	// TEMP TODO:REMOVE AND PUT SOMEWHERE RELEVANT
	if(Input::IsKeyPressed('1')) this->TEMPcurrentBlockID = GRASS;
	if(Input::IsKeyPressed('2')) this->TEMPcurrentBlockID = DIRT;
	if(Input::IsKeyPressed('3')) this->TEMPcurrentBlockID = STONE;
	if(Input::IsKeyPressed('4')) this->TEMPcurrentBlockID = BLACKSTONE;
	if(Input::IsKeyPressed('5')) this->TEMPcurrentBlockID = SAND;
	if(Input::IsKeyPressed('6')) this->TEMPcurrentBlockID = CLAY;
	if(Input::IsKeyPressed('7')) this->TEMPcurrentBlockID = WATER;
	if(Input::IsKeyPressed('8')) this->TEMPcurrentBlockID = OAK_LOG;
	if(Input::IsKeyPressed('9')) this->TEMPcurrentBlockID = OAK_PLANKS;
	if(Input::IsKeyPressed('0')) this->TEMPcurrentBlockID = OAK_LEAVES;
	




	// PLAYER RAYCAST

	VoxelRay ray(transform.position, transform.forward());

	//engine->GetCurrentScene()->GetObject3D("a_debug_look")->transform.position = ray.origin + (ray.direction * 1.f);

	Vector3Int lookHitPoint = Vector3Int(0,0,0);
	Vector3Int lookHitNormal;
	BlockID lookHitBlock;
	if(VoxelRay::Cast(&ray, chunkManager, 10.f, &lookHitPoint, &lookHitBlock, &lookHitNormal )) {
		engine->GetCurrentScene()->GetObject3D("a_debug_pos")->models[0]->alpha = 0.99f;
		engine->GetCurrentScene()->GetObject3D("a_debug_pos")->transform.position = Vector3((float)lookHitPoint.x, (float)lookHitPoint.y, (float)lookHitPoint.z) + Vector3(0.5f, 0.5f, 0.5f);
	
	
		// INPUT MODIFY
		if(Input::IsMouseLocked()) {
			if(Input::IsMouseKeyPressed(MOUSE_L)) {
				chunkManager->SetBlockAtWorldPos(lookHitPoint, AIR);
			}

			if(Input::IsMouseKeyPressed(MOUSE_R)) {
				AABB targetBlockAABB = AABB(lookHitPoint + lookHitNormal + Vector3(0.5f, 0.5f, 0.5f), Vector3(0.5f, 0.5f, 0.5f));
				if(!targetBlockAABB.Intersects(aabb)) {
					chunkManager->SetBlockAtWorldPos(lookHitPoint + lookHitNormal, TEMPcurrentBlockID);
				}
			}
		}
	}
	else {
		engine->GetCurrentScene()->GetObject3D("a_debug_pos")->models[0]->alpha = 0.0f;
	}

	




	// DEBUG INFO

	//engine->GetCurrentScene()->GetObject3D("cam_bounds")->transform.position = transform.position - Vector3(0, 0.62f, 0);

	engine->GetCurrentScene()->GetObject2D<Label>("fps-counter")->SetText(to_string(static_cast<int>(roundf(1.f/dTime))));

	string debugChunkData = "";
	////AcquireSRWLockExclusive()
	if(ChunkDatabase::Get()!=nullptr)
	for(const pair<tuple<int,int,int>,Chunk*>& pair : ChunkDatabase::Get()->chunkHash) {
		debugChunkData += ":"+Vector3Int(pair.first).ToString() + "\n";
	}

	//AcquireSRWLockExclusive(&ChunkDatabase::Get()->chunkHashMutex);
	Vector3Int camBlockPos = Vector3Int::FloorToInt(transform.position);
	engine->GetCurrentScene()->GetObject2D<Label>("worldpos")->SetText(
		transform.position.ToString() + "\n" + 
		//velocity.ToString() + "\n" + 
		to_string(isGrounded) + "\n" + "\n" +
		playerTopPos.ToString() + "\n" +
		playerBottomPos.ToString() + "\n" +
		lookHitPoint.ToString() + "\n" 
		+ debugChunkData
		//+ to_string(ChunkDatabase::Get()->chunkHash.size())

	);
	//ReleaseSRWLockExclusive(&ChunkDatabase::Get()->chunkHashMutex);
	//engine->GetCurrentScene()->GetObject2D<Label>("chunkpos")->SetText(ChunkManager::ToChunkIndexPosition(camBlockPos.x, camBlockPos.y, camBlockPos.z).ToString());
	//engine->GetCurrentScene()->GetObject2D<Label>("indexpos")->SetText(Vector3Int(FloorMod(camBlockPos.x, CHUNKSIZE_X), FloorMod(camBlockPos.y, CHUNKSIZE_Y), FloorMod(camBlockPos.z, CHUNKSIZE_Z)).ToString());


	// TODO: REMOVE
	// TEMP CROSSHAIR
	engine->GetCurrentScene()->GetObject3D("a_debug_look")->transform.position = transform.position + (transform.forward() * 0.1f);
	engine->GetCurrentScene()->GetObject3D("a_debug_look")->transform.rotation = transform.rotation;

	// KEEP AT END
	camera->transform = transform;
}

