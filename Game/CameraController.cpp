#include "CameraController.h"


void CameraController::Start()
{
}

void CameraController::Update(float dTime)
{
	Engine* engine = Engine::Get();

	float camSpeed = 5.f * dTime;

	//float horizontal = Input::GetInputAxis("Horizontal");
	//float vertical = Input::GetInputAxis("Vertical");

	Vector2 input = Input::GetInputVector();

	transform.TranslateLocal(camSpeed * input.x, 0, camSpeed * input.y);

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
	float lookSpeed = 5.f * dTime;

	//gfx.camera.AdjustRotation(mouseDelta.y * lookSpeed, mouseDelta.x * lookSpeed, 0.0f);
	if (Input::IsMouseLocked())
		transform.rotation += Vector3(mouseDelta.y * lookSpeed, mouseDelta.x * lookSpeed, 0.f);

	if (Input::IsKeyPressed(VK_ESCAPE)) {
		Input::SetMouseLocked(!Input::IsMouseLocked());
		transform.rotation = Vector3::Zero();
	}

	engine->GetGraphics()->camera.transform = transform;
}

