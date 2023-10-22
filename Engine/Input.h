#pragma once

#include <Windows.h>

#define RAWBUFF_SIZE 512
#define KEYBUFF_SIZE 255

class Input {
private:
	unsigned char inBuffer[RAWBUFF_SIZE];
	bool keyBuffer[KEYBUFF_SIZE]; //VKEY code indices
	bool prevKeyBuffer[KEYBUFF_SIZE];


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
			MessageBox(0, L"RAWBUFF TOO SMALL", 0, 0);
			//assert(false);
			return; //Buffer too small
		}

		UINT result = GetRawInputData(input, RID_INPUT, Input::_Instance->inBuffer, &dwSize, sizeof(RAWINPUTHEADER));
		if(result != dwSize) {
			MessageBox(0, L"Get Raw Input Failed", 0, 0);
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

	/// <summary>
	/// !!  !! MUST call at the very end of every frame in order to service for IsKeyPressed and IsKeyReleased
	/// </summary>
	static void EndUpdate() {
		memcpy(&Input::_Instance->prevKeyBuffer, &Input::_Instance->keyBuffer, sizeof(bool) * KEYBUFF_SIZE); //Create duplicate of keybuffer for previous frame
	}

	static void Init(HWND hwnd) {
		if(_Instance == nullptr) {
			_Instance = new Input();
		}

		RAWINPUTDEVICE rid;
		rid.usUsagePage = 0x01;
		rid.usUsage = 0x06;
		rid.dwFlags = RIDEV_INPUTSINK;
		rid.hwndTarget = hwnd;

		if(!RegisterRawInputDevices(&rid, 1, sizeof(rid))) {
			exit(70);
		}
	}
};