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

	static void UpdatePads() {
		for (DWORD i = 0; i < 4; i++) {
			GamepadState& state = _Instance->gamepads[i];
			state.port = -1;
			ZeroMemory(&state.xState, sizeof(XINPUT_STATE));
			if (XInputGetState(i, &state.xState) == ERROR_SUCCESS) {
				Vector2 lStick = Vector2((float)state.xState.Gamepad.sThumbLX / 32767.f, (float)state.xState.Gamepad.sThumbLY / 32767.f);
				Vector2 rStick = Vector2((float)state.xState.Gamepad.sThumbRX / 32767.f, (float)state.xState.Gamepad.sThumbRY / 32767.f);
				state.leftTrigger = (float)state.xState.Gamepad.bLeftTrigger / 255.f;
				state.rightTrigger = (float)state.xState.Gamepad.bRightTrigger / 255.f;
				state.port = i;

				state.leftStick = lStick;
				state.rightStick = rStick;
			}
		}
	}

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



	static void HandleRawInput(HRAWINPUT input) {

		UINT dwSize = 0;
		UINT res = GetRawInputData(input, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
		if (res != 0)
			return; //Ignore bad input

		if (dwSize > RAWBUFF_SIZE) {
			//MessageBox(0, L"RAWBUFF TOO SMALL", 0, 0);
			//assert(false);
			return; //Buffer too small
		}

		UINT result = GetRawInputData(input, RID_INPUT, Input::_Instance->inBuffer, &dwSize, sizeof(RAWINPUTHEADER));
		if (result != dwSize) {
			//MessageBox(0, L"Get Raw Input Failed", 0, 0);
			//assert(false);
			return; //GetRawInput failed
		}
		//MessageBox(0, L"Input received", 0, 0);

		RAWINPUT* raw = (RAWINPUT*)Input::_Instance->inBuffer;

		if (raw->header.dwType == RIM_TYPEKEYBOARD) {
			USHORT vkey = raw->data.keyboard.VKey;
			if (vkey < 255) {
				USHORT flags = raw->data.keyboard.Flags;
				if (flags & RI_KEY_BREAK) { // Key released
					Input::_Instance->keyBuffer[vkey] = false;
				}
				else { // Key pressed
					Input::_Instance->keyBuffer[vkey] = true;
				}
			}
		}
		else if (raw->header.dwType == RIM_TYPEMOUSE) {
			USHORT flags = raw->data.mouse.usButtonFlags;



			if (flags & RI_MOUSE_LEFT_BUTTON_DOWN)  // 1 =		0000000001 = 1
				_Instance->mKeyBuffer[MOUSE_L] = true;
			if (flags & RI_MOUSE_LEFT_BUTTON_UP)  // 2 =		0000000010 = 2
				_Instance->mKeyBuffer[MOUSE_L] = false;
			if (flags & RI_MOUSE_RIGHT_BUTTON_DOWN)  // 4 =		0000000100 = 3
				_Instance->mKeyBuffer[MOUSE_R] = true;
			if (flags & RI_MOUSE_RIGHT_BUTTON_UP)  // 8 =		0000001000 = 4
				_Instance->mKeyBuffer[MOUSE_R] = false;
			if (flags & RI_MOUSE_MIDDLE_BUTTON_DOWN)  // 16 =	0000010000 = 5
				_Instance->mKeyBuffer[MOUSE_M] = true;
			if (flags & RI_MOUSE_MIDDLE_BUTTON_UP)  // 32 =		0000100000 = 6
				_Instance->mKeyBuffer[MOUSE_M] = false;
			if (flags & RI_MOUSE_BUTTON_4_DOWN)  // 64 =		0001000000 = 7
				_Instance->mKeyBuffer[MOUSE_4] = true;
			if (flags & RI_MOUSE_BUTTON_4_UP)  // 128 =			0010000000 = 8
				_Instance->mKeyBuffer[MOUSE_4] = false;
			if (flags & RI_MOUSE_BUTTON_5_DOWN)  // 256 =		0100000000 = 9
				_Instance->mKeyBuffer[MOUSE_5] = true;
			if (flags & RI_MOUSE_BUTTON_5_UP)  // 512 =			1000000000 = 10
				_Instance->mKeyBuffer[MOUSE_5] = false;

		}

	}

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

	static void GetMouseInformation() {
		if (_Instance->lastMouseGetAge == _Instance->inputAge)
			return; // The current mouse information is up to date, so exit

		POINT mPos;
		if (GetCursorPos(&mPos)) {
			if (ScreenToClient(_Instance->hWnd, &mPos)) {
				_Instance->mousePos = XMFLOAT2((float)mPos.x, (float)mPos.y);

				_Instance->lastMouseGetAge = _Instance->inputAge;
			}
		}
	}

	static XMFLOAT2 MouseDelta() {
		GetMouseInformation();
		return XMFLOAT2(_Instance->mousePos.x - _Instance->prevMousePos.x, _Instance->mousePos.y - _Instance->prevMousePos.y);
		//return XMFLOAT2(_Instance->mousePos.x - (_Instance->newMouseClip.right / 2), _Instance->mousePos.y - (_Instance->newMouseClip.bottom / 2));
	}

	static XMFLOAT2 MousePosition() {
		GetMouseInformation();
		return _Instance->mousePos;
	}

	static void SetMouseLocked(bool isLocked) {
		_Instance->isMouseLocked = isLocked;
		//ShowCursor(isLocked);
		GetClipCursor(&_Instance->oldMouseClip);
		if (isLocked) {
			//while (ShowCursor(false) >= 0);
			GetWindowRect(_Instance->hWnd, &_Instance->newMouseClip);
			ClipCursor(&_Instance->newMouseClip);
		}
		else {
			//while (ShowCursor(true) <= 0);
			ClipCursor(NULL);
		}
	}

	static bool IsMouseLocked() {
		return _Instance->isMouseLocked;
	}

	//static void CreateVectorMap(string mapName, USHORT up, USHORT down, USHORT left, USHORT right) {
	//	//_Instance->axisMap[mapName] = {up, down, left, right};
	//	USHORT map[4] = { up, down, left, right };
	//	_Instance->axisMap[mapName] = map;
	//}

	//todo: make ways to create more input axes
	// something akin to the new unity inputsystem?
	static const float GetInputAxis(string axisName, int controllerIdx = 0) {
		if (axisName == "Horizontal" || axisName == "horizontal") {
			return ((float)Input::IsKeyHeld('D') - (float)Input::IsKeyHeld('A')) + LeftAxis(controllerIdx).x;
		}
		else {
			return ((float)Input::IsKeyHeld('W') - (float)Input::IsKeyHeld('S')) + LeftAxis(controllerIdx).y;
		}
	}

	static const Vector2 GetInputVector() {
		return Vector2(GetInputAxis("Horizontal"), GetInputAxis("Vertical"));
	}

	//static const 

	//static const Vector2 GetMoveAxis(int idx = 0) {
	//	return Vector2(max(LeftAxis().x, ))
	//}

	/// <summary>
	/// !!  !! MUST call at the very end of every frame in order to service for IsKeyPressed and IsKeyReleased
	/// </summary>
	static void EndUpdate() {
		memcpy(&_Instance->prevKeyBuffer, &_Instance->keyBuffer, sizeof(bool) * KEYBUFF_SIZE); //Create duplicate of keybuffer for previous frame
		memcpy(&_Instance->prevmKeyBuffer, &_Instance->mKeyBuffer, sizeof(bool) * MKEYBUFF_SIZE);

		_Instance->prevMousePos = _Instance->mousePos;

		if (_Instance->isMouseLocked) {
			SetCursorPos(_Instance->newMouseClip.right / 2, _Instance->newMouseClip.bottom / 2);
		}

		_Instance->inputAge++;


		UpdatePads();
	}

	static void Init(HWND hwnd) {
		if (_Instance == nullptr) {
			_Instance = new Input();
		}

		_Instance->hWnd = hwnd;


		RAWINPUTDEVICE devices[2];
		//Keyboard
		devices[0].usUsagePage = 0x01;
		devices[0].usUsage = 0x06;
		devices[0].dwFlags = RIDEV_INPUTSINK;
		devices[0].hwndTarget = hwnd;

		//Mouse
		devices[1].usUsagePage = 0x01;
		devices[1].usUsage = 0x02;
		devices[1].dwFlags = RIDEV_INPUTSINK;
		devices[1].hwndTarget = hwnd;

		if (!RegisterRawInputDevices(devices, 2, sizeof(devices[0]))) {
			//exit(70);
			MessageBox(0, L"Failed to register input devices", 0, 0);
		}

		GetMouseInformation();
		_Instance->prevMousePos = _Instance->mousePos; //Avoids a sudden perceived jump from 0,0 (default) to 1xxx,1xxx (actual position) at startup

		//CreateVectorMap("Horizontal", 'W', 'S', 'A', 'D');
	}

	//~Input() {
	//	for (pair<string, USHORT*> pair : _Instance->axisMap) {
	//		delete[4] pair.second;
	//	}
	//}
};