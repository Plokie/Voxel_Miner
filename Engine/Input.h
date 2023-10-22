#pragma once

#include <Windows.h>
#include <DirectXMath.h>

#define RAWBUFF_SIZE 512
#define KEYBUFF_SIZE 255

using namespace DirectX;

class Input {
private:
	unsigned char inBuffer[RAWBUFF_SIZE];
	bool keyBuffer[KEYBUFF_SIZE]; //VKEY code indices
	bool prevKeyBuffer[KEYBUFF_SIZE];

	bool isMouseLocked = false;

	HWND hWnd;
	unsigned int inputAge = 0, lastMouseGetAge=-1;

	RECT oldMouseClip, newMouseClip;

	XMFLOAT2 prevMousePos, mousePos;

	Input(){
		ZeroMemory(&inBuffer, sizeof(unsigned char) * RAWBUFF_SIZE);
		ZeroMemory(&keyBuffer, sizeof(bool) * KEYBUFF_SIZE);
		ZeroMemory(&prevKeyBuffer, sizeof(bool) * KEYBUFF_SIZE);
	}
	static Input* _Instance;
public:
	Input(Input& other) = delete;
	void operator=(const Input&) = delete;


	static void HandleRawInput(HRAWINPUT input) {

		UINT dwSize = 0;
		UINT res = GetRawInputData(input, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
		if(res != 0) 
			return; //Ignore bad input

		if(dwSize > RAWBUFF_SIZE) {
			//MessageBox(0, L"RAWBUFF TOO SMALL", 0, 0);
			//assert(false);
			return; //Buffer too small
		}

		UINT result = GetRawInputData(input, RID_INPUT, Input::_Instance->inBuffer, &dwSize, sizeof(RAWINPUTHEADER));
		if(result != dwSize) {
			//MessageBox(0, L"Get Raw Input Failed", 0, 0);
			//assert(false);
			return; //GetRawInput failed
		}
		//MessageBox(0, L"Input received", 0, 0);

		RAWINPUT* raw = (RAWINPUT*)Input::_Instance->inBuffer;

		if(raw->header.dwType == RIM_TYPEKEYBOARD) {
			USHORT vkey = raw->data.keyboard.VKey;
			if(vkey < 255) {
				USHORT flags = raw->data.keyboard.Flags;
				if(flags & RI_KEY_BREAK) { // Key released
					Input::_Instance->keyBuffer[vkey] = false;
				}
				else { // Key pressed
					Input::_Instance->keyBuffer[vkey] = true;
				}
			}
		}
		else if(raw->header.dwType == RIM_TYPEMOUSE) {
			USHORT flags = raw->data.mouse.usButtonFlags;

			if(flags & RI_MOUSE_LEFT_BUTTON_DOWN) {
			}
			if(flags & RI_MOUSE_LEFT_BUTTON_UP) {
			}
			if(flags & RI_MOUSE_MIDDLE_BUTTON_DOWN) {
			}
			if(flags & RI_MOUSE_MIDDLE_BUTTON_UP) {
			}
			if(flags & RI_MOUSE_RIGHT_BUTTON_DOWN) {
			}
			if(flags & RI_MOUSE_RIGHT_BUTTON_UP) {
			}


		}

	}

	static bool IsKeyPressed(USHORT VKEY) {
		return Input::_Instance->keyBuffer[VKEY] && !Input::_Instance->prevKeyBuffer[VKEY];
	}

	static bool IsKeyReleased(USHORT VKEY) {
		return !Input::_Instance->keyBuffer[VKEY] && Input::_Instance->prevKeyBuffer[VKEY];
	}

	static bool IsKeyHeld(USHORT VKEY) {
		return Input::_Instance->keyBuffer[VKEY];
	}

	static void GetMouseInformation() {
		if(_Instance->lastMouseGetAge == _Instance->inputAge)
			return; // The current mouse information is up to date, so exit

		POINT mPos;
		if(GetCursorPos(&mPos)) {
			if(ScreenToClient(_Instance->hWnd, &mPos)) {
				_Instance->mousePos = XMFLOAT2((float)mPos.x, (float)mPos.y);
				_Instance->lastMouseGetAge = _Instance->inputAge;
			}
		}
	}

	static XMFLOAT2 MouseDelta() {
		GetMouseInformation();
		return XMFLOAT2(_Instance->mousePos.x - _Instance->prevMousePos.x, _Instance->mousePos.y - _Instance->prevMousePos.y);
	}

	static XMFLOAT2 MousePosition() {
		GetMouseInformation();
		return _Instance->mousePos;
	}

	static void SetMouseLocked(bool isLocked) {
		_Instance->isMouseLocked = isLocked;
		//ShowCursor(isLocked);
		GetClipCursor(&_Instance->oldMouseClip);
		if(isLocked) {
			while(ShowCursor(false) >= 0);
			GetWindowRect(_Instance->hWnd, &_Instance->newMouseClip);
			ClipCursor(&_Instance->newMouseClip);
		}
		else {
			while(ShowCursor(true) <= 0);
			ClipCursor(NULL);
		}
	}

	static bool GetMouseLocked() {
		return _Instance->isMouseLocked;
	}

	/// <summary>
	/// !!  !! MUST call at the very end of every frame in order to service for IsKeyPressed and IsKeyReleased
	/// </summary>
	static void EndUpdate() {
		memcpy(&Input::_Instance->prevKeyBuffer, &Input::_Instance->keyBuffer, sizeof(bool) * KEYBUFF_SIZE); //Create duplicate of keybuffer for previous frame

		_Instance->prevMousePos = _Instance->mousePos;

		_Instance->inputAge++;

		if(_Instance->isMouseLocked) {
			SetCursorPos(_Instance->newMouseClip.right / 2.0f, _Instance->newMouseClip.bottom / 2.0f);
		}
	}

	static void Init(HWND hwnd) {
		if(_Instance == nullptr) {
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

		if(!RegisterRawInputDevices(devices, 2, sizeof(devices[0]))) {
			exit(70);
		}
	}
};