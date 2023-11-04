#include "CameraController.h"

#include "VoxelRaycast.h"

void CameraController::Start()
{
	Engine* engine = Engine::Get();

	engine->GetCurrentScene()->CreateObject3D(new Object3D(), "a_debug_pos", "cube", "block-select");
	engine->GetCurrentScene()->GetObject3D("a_debug_pos")->models[0]->SetTransparent(true);
	engine->GetCurrentScene()->GetObject3D("a_debug_pos")->transform.scale = Vector3(0.51f, 0.51f, 0.51f);
}

void CameraController::Update(float dTime)
{
	Engine* engine = Engine::Get();
	Camera* camera = &engine->GetGraphics()->camera;

	float camSpeed = 15.f * dTime;

	//float horizontal = Input::GetInputAxis("Horizontal");
	//float vertical = Input::GetInputAxis("Vertical");

	Vector2 input = Input::GetInputVector();

	Vector3 moveAxis = transform.basis(camSpeed * input.x, 0, camSpeed * input.y);
	moveAxis.y = 0.0;
	//moveAxis = moveAxis.normalized();

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
	float lookSpeed = 0.4f * dTime;

	//gfx.camera.AdjustRotation(mouseDelta.y * lookSpeed, mouseDelta.x * lookSpeed, 0.0f);
	if (Input::IsMouseLocked())
		transform.rotation += Vector3(mouseDelta.y * lookSpeed, mouseDelta.x * lookSpeed, 0.f);

	if (Input::IsKeyPressed(VK_ESCAPE)) {
		Input::SetMouseLocked(!Input::IsMouseLocked());
	}

	camera->transform = transform;

	VoxelRay ray(camera->transform.position, camera->transform.forward());

	Vector3Int lookHitPoint;
	if(VoxelRay::Cast(&ray, (ChunkManager*)engine->GetCurrentScene()->GetObject3D("ChunkManager"), 10.f, &lookHitPoint)) {
		engine->GetCurrentScene()->GetObject3D("a_debug_pos")->transform.position = Vector3((float)lookHitPoint.x, (float)lookHitPoint.y, (float)lookHitPoint.z) + Vector3(0.5f,0.5f,0.5f);
	}

	
}

