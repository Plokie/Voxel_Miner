#pragma once

#include <Windows.h>
#include <DirectXMath.h>

#define RAWBUFF_SIZE 512
#define KEYBUFF_SIZE 255
#define MKEYBUFF_SIZE 5

using namespace DirectX;

typedef enum { MOUSE_L, MOUSE_R, MOUSE_M, MOUSE_4, MOUSE_5 };

class Input {
private:
	unsigned char inBuffer[RAWBUFF_SIZE];
	bool keyBuffer[KEYBUFF_SIZE]; //VKEY code indices
	bool prevKeyBuffer[KEYBUFF_SIZE];

	bool mKeyBuffer[MKEYBUFF_SIZE];
	bool prevmKeyBuffer[MKEYBUFF_SIZE];

	bool isMouseLocked = false;

	HWND hWnd;
	unsigned int inputAge = 0, lastMouseGetAge=-1;

	RECT oldMouseClip, newMouseClip;

	XMFLOAT2 prevMousePos, mousePos;

	Input(){
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

				

			if(flags & RI_MOUSE_LEFT_BUTTON_DOWN)  // 1 =		0000000001 = 1
				_Instance->mKeyBuffer[MOUSE_L] = true;
			if(flags & RI_MOUSE_LEFT_BUTTON_UP)  // 2 =		0000000010 = 2
				_Instance->mKeyBuffer[MOUSE_L] = false;
			if(flags & RI_MOUSE_RIGHT_BUTTON_DOWN)  // 4 =		0000000100 = 3
				_Instance->mKeyBuffer[MOUSE_R] = true;
			if(flags & RI_MOUSE_RIGHT_BUTTON_UP)  // 8 =		0000001000 = 4
				_Instance->mKeyBuffer[MOUSE_R] = false;
			if(flags & RI_MOUSE_MIDDLE_BUTTON_DOWN)  // 16 =	0000010000 = 5
				_Instance->mKeyBuffer[MOUSE_M] = true;
			if(flags & RI_MOUSE_MIDDLE_BUTTON_UP)  // 32 =		0000100000 = 6
				_Instance->mKeyBuffer[MOUSE_M] = false;
			if(flags & RI_MOUSE_BUTTON_4_DOWN)  // 64 =		0001000000 = 7
				_Instance->mKeyBuffer[MOUSE_4] = true;
			if(flags & RI_MOUSE_BUTTON_4_UP)  // 128 =			0010000000 = 8
				_Instance->mKeyBuffer[MOUSE_4] = false;
			if(flags & RI_MOUSE_BUTTON_5_DOWN)  // 256 =		0100000000 = 9
				_Instance->mKeyBuffer[MOUSE_5] = true;
			if(flags & RI_MOUSE_BUTTON_5_UP)  // 512 =			1000000000 = 10
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
		memcpy(&_Instance->prevKeyBuffer, &_Instance->keyBuffer, sizeof(bool) * KEYBUFF_SIZE); //Create duplicate of keybuffer for previous frame
		memcpy(&_Instance->prevmKeyBuffer, &_Instance->mKeyBuffer, sizeof(bool) * MKEYBUFF_SIZE);

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

		GetMouseInformation();
		_Instance->prevMousePos = _Instance->mousePos; //Avoids a sudden perceived jump from 0,0 (default) to 1xxx,1xxx (actual position) at startup
	}
};