#include "PlayerController.h"

#include "VoxelRaycast.h"
#include "../Engine/Label.h"
#include "ChunkManager.h"

void CameraController::Start()
{
	Engine* engine = Engine::Get();

	engine->GetCurrentScene()->CreateObject3D(new Object3D(), "a_debug_pos", "cube", "block-select");
	engine->GetCurrentScene()->GetObject3D("a_debug_pos")->models[0]->SetTransparent(true);
	engine->GetCurrentScene()->GetObject3D("a_debug_pos")->transform.scale = Vector3(0.51f, 0.51f, 0.51f);

	Vector3 playerHalfExtents = Vector3(0.3f, 1.f, 0.3f);

	engine->GetCurrentScene()->CreateObject3D(new Object3D(), "cam_bounds", "inverse-cube", "block-select");
	engine->GetCurrentScene()->GetObject3D("cam_bounds")->models[0]->SetTransparent(true);
	engine->GetCurrentScene()->GetObject3D("cam_bounds")->transform.scale = Vector3(playerHalfExtents);

	aabb = AABB(transform.position, playerHalfExtents * 1.5f); //hmm, i dont know why i have to multilpy 1.5 todo: look into this
}

void CameraController::Update(float dTime)
{
	Engine* engine = Engine::Get();
	Camera* camera = &engine->GetGraphics()->camera;

	float camSpeed = 2.f * dTime;

	//float horizontal = Input::GetInputAxis("Horizontal");
	//float vertical = Input::GetInputAxis("Vertical");

	Vector2 input = Input::GetInputVector().normalized();

	Vector3 moveAxis = transform.basis(input.x, 0, input.y);
	moveAxis.y = 0.0;
	moveAxis = moveAxis.normalized();
	moveAxis *= camSpeed;

	transform.position += moveAxis;

	if (Input::IsKeyHeld(VK_SPACE)) {
		transform.position += Vector3(0, camSpeed, 0);
	}
	if (Input::IsKeyHeld(VK_SHIFT)) {
		transform.position -= Vector3(0, camSpeed, 0);
	}

	if (Input::IsPadButtonHeld(0, XINPUT_GAMEPAD_A)) {
		transform.position += Vector3(0, camSpeed, 0);
	}

	XMFLOAT2 mouseDelta = Input::MouseDelta();
	float lookSpeed = 0.0025f; // The polling rate of the mouse is already tied to the framerate, do no dt
	

	if (Input::IsMouseLocked())
		transform.rotation += Vector3(mouseDelta.y * lookSpeed, mouseDelta.x * lookSpeed, 0.f);

	if (Input::IsKeyPressed(VK_ESCAPE)) {
		Input::SetMouseLocked(!Input::IsMouseLocked());
	}


	VoxelRay ray(transform.position, transform.forward());

	Vector3Int lookHitPoint;
	if(VoxelRay::Cast(&ray, (ChunkManager*)engine->GetCurrentScene()->GetObject3D("ChunkManager"), 10.f, &lookHitPoint)) {
		engine->GetCurrentScene()->GetObject3D("a_debug_pos")->transform.position = Vector3((float)lookHitPoint.x, (float)lookHitPoint.y, (float)lookHitPoint.z) + Vector3(0.5f,0.5f,0.5f);
	}

	aabb.SetPosition(transform.position - Vector3(0, 0.62f, 0));

	//todo: move to appropriate location elsewhere, just here for the sake of testing
	AABB groundAABB = AABB(Vector3(0, -1.f, 0), Vector3(30.f, 0.5f, 30.f));
	if(aabb.Intersects(groundAABB)) {
		transform.position += AABB::penetration_vector(AABB::minkowski_difference(groundAABB, aabb));
	}


	engine->GetCurrentScene()->GetObject3D("cam_bounds")->transform.position = transform.position - Vector3(0, 0.62f, 0);

	engine->GetCurrentScene()->GetObject2D<Label>("fps-counter")->SetText(to_string(static_cast<int>(roundf(1.f/dTime))));

	Vector3Int camBlockPos = Vector3Int::FloorToInt(transform.position);
	engine->GetCurrentScene()->GetObject2D<Label>("worldpos")->SetText(camBlockPos.ToString());
	//engine->GetCurrentScene()->GetObject2D<Label>("chunkpos")->SetText(ChunkManager::ToChunkIndexPosition(camBlockPos.x, camBlockPos.y, camBlockPos.z).ToString());
	//engine->GetCurrentScene()->GetObject2D<Label>("indexpos")->SetText(Vector3Int(FloorMod(camBlockPos.x, CHUNKSIZE_X), FloorMod(camBlockPos.y, CHUNKSIZE_Y), FloorMod(camBlockPos.z, CHUNKSIZE_Z)).ToString());

	// KEEP AT END
	camera->transform = transform;
}

