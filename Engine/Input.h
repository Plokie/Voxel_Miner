#pragma once

#include <Windows.h>
#include <Xinput.h>
#include <map>
#include <Mouse.h>
#include <functional>
#include <string>

#pragma comment(lib,"XInput.lib")
#pragma comment(lib,"Xinput9_1_0.lib")

#include "MathUtil.h"

#define RAWBUFF_SIZE 512
#define KEYBUFF_SIZE 255
#define MKEYBUFF_SIZE 5
#define GAMEPAD_COUNT 4

using namespace DirectX;

enum { MOUSE_L, MOUSE_R, MOUSE_M, MOUSE_4, MOUSE_5 };

struct GamepadState {
	int port = -1;
	Vector2 leftStick = Vector2();
	Vector2 rightStick = Vector2();
	Vector2 deadzone = Vector2(0.125f, 0.125f);
	float leftTrigger = 0.0f;
	float rightTrigger = 0.0f;


	XINPUT_STATE xState = {};
};


class Input {
private:
	unsigned char inBuffer[RAWBUFF_SIZE];
	bool keyBuffer[KEYBUFF_SIZE]; //VKEY code indices
	bool prevKeyBuffer[KEYBUFF_SIZE];

	bool mKeyBuffer[MKEYBUFF_SIZE];
	bool prevmKeyBuffer[MKEYBUFF_SIZE];

	WORD prevGamepadButtons[GAMEPAD_COUNT];
	bool prevLeftTriggerHeld[GAMEPAD_COUNT];
	bool prevRightTriggerHeld[GAMEPAD_COUNT];
	bool virtualMouseEnabled = false;
	Vector2 virtualMouseDelta = Vector2(0.f,0.f);

	bool isMouseLocked = false;
	int scrollDelta = 0;

	// Used for reading keyboard inputs as they come in (text input for example)
	map<void*, function<void(unsigned short)>> inputStreams = {};

	HWND hWnd;
	unsigned int inputAge = 0, lastMouseGetAge = -1;
	//map<string, USHORT*> axisMap = {};

	RECT oldMouseClip, newMouseClip;

	XMFLOAT2 prevMousePos, mousePos;
	XMFLOAT2 mouseDelta;

	Input() {
		ZeroMemory(&inBuffer, sizeof(unsigned char) * RAWBUFF_SIZE);

		ZeroMemory(&keyBuffer, sizeof(bool) * KEYBUFF_SIZE);
		ZeroMemory(&prevKeyBuffer, sizeof(bool) * KEYBUFF_SIZE);

		ZeroMemory(&mKeyBuffer, sizeof(bool) * 5);
		ZeroMemory(&prevmKeyBuffer, sizeof(bool) * 5);

		ZeroMemory(&prevGamepadButtons, sizeof(WORD) * GAMEPAD_COUNT);
		ZeroMemory(&prevLeftTriggerHeld, sizeof(bool) * GAMEPAD_COUNT);
		ZeroMemory(&prevRightTriggerHeld, sizeof(bool) * GAMEPAD_COUNT);
	}
	static Input* _Instance;
public:
	Input(Input& other) = delete;
	void operator=(const Input&) = delete;

	GamepadState gamepads[GAMEPAD_COUNT];

	static void UpdatePads();

	static bool IsPadButtonHeld(int idx, USHORT KEY);
	static bool IsPadButtonPressed(int idx, USHORT KEY);

	static float GetPadLeftTrigger(int idx);
	static float GetPadRightTrigger(int idx);

	static bool IsLeftTriggerHeld(int idx);
	static bool IsRightTriggerHeld(int idx);

	static bool IsLeftTriggerPressed(int idx);
	static bool IsRightTriggerPressed(int idx);

	static bool IsLeftTriggerReleased(int idx);
	static bool IsRightTriggerReleased(int idx);

	static const Vector2& LeftAxis(int idx);

	static const Vector2& RightAxis(int idx);

	static void HandleRawInput(HRAWINPUT input);
	
	static void HandleCharInput(WPARAM key);

	static void HandleMouseWheel(WPARAM wparam);

	static bool IsKeyPressed(USHORT VKEY) {
		return _Instance->keyBuffer[VKEY] && !_Instance->prevKeyBuffer[VKEY];
	}

	static bool IsKeyReleased(USHORT VKEY) {
		return !_Instance->keyBuffer[VKEY] && _Instance->prevKeyBuffer[VKEY];
	}

	static bool IsKeyHeld(USHORT VKEY) {
		return Input::_Instance->keyBuffer[VKEY];
	}

	static bool IsMouseKeyPressed(USHORT MKEY) {
		return _Instance->mKeyBuffer[MKEY] && !_Instance->prevmKeyBuffer[MKEY];
	}

	static bool IsMouseKeyReleased(USHORT MKEY) {
		return !_Instance->mKeyBuffer[MKEY] && _Instance->prevmKeyBuffer[MKEY];
	}

	static bool IsMouseKeyHeld(USHORT MKEY) {
		return _Instance->mKeyBuffer[MKEY];
	}

	static void SetVirtualMouse(bool state);

	static void GetMouseInformation();

	static XMFLOAT2 ScreenCenter();

	static XMFLOAT2 MouseDelta();

	static XMFLOAT2 MousePosition();
	static Vector2 MousePositionVec();

	static void SetMouseLocked(bool isLocked);
	

	static bool IsMouseLocked();

	static void OpenInputStream(void* owner, function<void(unsigned short)> func);
	static void CloseInputStream(void* owner);

	//todo: make ways to create more input axes
	// something akin to the new unity inputsystem?
	static const float GetInputAxis(string axisName, int controllerIdx = 0);

	static const int GetMouseScrollDelta();

	static const Vector2 GetInputVector();

	//static const 

	//static const Vector2 GetMoveAxis(int idx = 0) {
	//	return Vector2(max(LeftAxis().x, ))
	//}

	

	/// <summary>
	/// !!  !! MUST call at the very end of every frame in order to service for IsKeyPressed and IsKeyReleased
	/// </summary>
	static void EndUpdate(float dt);

	static void Init(HWND hwnd);
};