#pragma once

#include <Windows.h>
#include <Xinput.h>
#include <Mouse.h>
#include <string>

#pragma comment(lib,"XInput.lib")
#pragma comment(lib,"Xinput9_1_0.lib")

#include "MathUtil.h"

#define RAWBUFF_SIZE 512
#define KEYBUFF_SIZE 255
#define MKEYBUFF_SIZE 5

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

	bool isMouseLocked = false;

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
	}
	static Input* _Instance;
public:
	Input(Input& other) = delete;
	void operator=(const Input&) = delete;

	GamepadState gamepads[4];

	static void UpdatePads();

	static bool IsPadButtonHeld(int idx, USHORT KEY) {
		if (_Instance->gamepads[idx].port != -1) {
			return (_Instance->gamepads[idx].xState.Gamepad.wButtons & KEY) != 0;
		}
		return false;
	}

	static const Vector2& LeftAxis(int idx) {
		return _Instance->gamepads[idx].leftStick;
	};

	static const Vector2& RightAxis(int idx) {
		return _Instance->gamepads[idx].rightStick;
	};

	static void HandleRawInput(HRAWINPUT input);
	

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

	static void GetMouseInformation();

	static XMFLOAT2 ScreenCenter();

	static XMFLOAT2 MouseDelta();

	static XMFLOAT2 MousePosition();

	static void SetMouseLocked(bool isLocked);
	

	static bool IsMouseLocked();

	//todo: make ways to create more input axes
	// something akin to the new unity inputsystem?
	static const float GetInputAxis(string axisName, int controllerIdx = 0);

	static const Vector2 GetInputVector();

	//static const 

	//static const Vector2 GetMoveAxis(int idx = 0) {
	//	return Vector2(max(LeftAxis().x, ))
	//}

	

	/// <summary>
	/// !!  !! MUST call at the very end of every frame in order to service for IsKeyPressed and IsKeyReleased
	/// </summary>
	static void EndUpdate();

	static void Init(HWND hwnd);
};