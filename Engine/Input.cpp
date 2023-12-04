#include "Input.h"

Input* Input::_Instance;

void Input::UpdatePads() {
	for(DWORD i = 0; i < 4; i++) {
		GamepadState& state = _Instance->gamepads[i];
		state.port = -1;
		ZeroMemory(&state.xState, sizeof(XINPUT_STATE));
		if(XInputGetState(i, &state.xState) == ERROR_SUCCESS) {
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

void Input::HandleRawInput(HRAWINPUT input) {
	UINT dwSize = 0;
	UINT res = GetRawInputData(input, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
	if(res != 0) return; //Ignore bad input

	if(dwSize > RAWBUFF_SIZE) {
		//MessageBox(0, L"RAWBUFF TOO SMALL", 0, 0);
		return; //Buffer too small
	}

	UINT result = GetRawInputData(input, RID_INPUT, Input::_Instance->inBuffer, &dwSize, sizeof(RAWINPUTHEADER));
	if(result != dwSize) {
		return; //GetRawInput failed
	}

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
		if(flags & RI_MOUSE_LEFT_BUTTON_UP)  // 2 =			0000000010 = 2
			_Instance->mKeyBuffer[MOUSE_L] = false;
		if(flags & RI_MOUSE_RIGHT_BUTTON_DOWN)  // 4 =		0000000100 = 3
			_Instance->mKeyBuffer[MOUSE_R] = true;
		if(flags & RI_MOUSE_RIGHT_BUTTON_UP)  // 8 =		0000001000 = 4
			_Instance->mKeyBuffer[MOUSE_R] = false;
		if(flags & RI_MOUSE_MIDDLE_BUTTON_DOWN)  // 16 =	0000010000 = 5
			_Instance->mKeyBuffer[MOUSE_M] = true;
		if(flags & RI_MOUSE_MIDDLE_BUTTON_UP)  // 32 =		0000100000 = 6
			_Instance->mKeyBuffer[MOUSE_M] = false;
		if(flags & RI_MOUSE_BUTTON_4_DOWN)  // 64 =			0001000000 = 7
			_Instance->mKeyBuffer[MOUSE_4] = true;
		if(flags & RI_MOUSE_BUTTON_4_UP)  // 128 =			0010000000 = 8
			_Instance->mKeyBuffer[MOUSE_4] = false;
		if(flags & RI_MOUSE_BUTTON_5_DOWN)  // 256 =		0100000000 = 9
			_Instance->mKeyBuffer[MOUSE_5] = true;
		if(flags & RI_MOUSE_BUTTON_5_UP)  // 512 =			1000000000 = 10
			_Instance->mKeyBuffer[MOUSE_5] = false;

		_Instance->mouseDelta = XMFLOAT2(_Instance->mouseDelta.x + static_cast<float>(raw->data.mouse.lLastX), _Instance->mouseDelta.y + static_cast<float>(raw->data.mouse.lLastY));
	}
}

void Input::HandleCharInput(WPARAM key)
{
	if(!_Instance->inputStreams.empty()) {
		for(auto& kvp : _Instance->inputStreams) {
			kvp.second(key);
		}
	}
}

void Input::GetMouseInformation() {
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

XMFLOAT2 Input::ScreenCenter() {
	return XMFLOAT2((_Instance->newMouseClip.right / 2.f) + _Instance->newMouseClip.left, (_Instance->newMouseClip.bottom / 2.f) + _Instance->newMouseClip.top);
}

XMFLOAT2 Input::MouseDelta() {
	return _Instance->mouseDelta;
}

XMFLOAT2 Input::MousePosition() {
	GetMouseInformation();
	return _Instance->mousePos;
}

void Input::SetMouseLocked(bool isLocked) {
	_Instance->isMouseLocked = isLocked;
	//ShowCursor(isLocked);
	GetClipCursor(&_Instance->oldMouseClip);
	if(isLocked) {
		while (ShowCursor(false) >= 0);
		GetWindowRect(_Instance->hWnd, &_Instance->newMouseClip);
		ClipCursor(&_Instance->newMouseClip);
	}
	else {
		while(ShowCursor(true) <= 0);
		ClipCursor(NULL);
	}
}

bool Input::IsMouseLocked() {
	return _Instance->isMouseLocked;
}

void Input::OpenInputStream(void* owner, function<void(unsigned short)> func)
{
	if(_Instance->inputStreams.find(owner) != _Instance->inputStreams.end()) {
		CloseInputStream(owner);
	}
	_Instance->inputStreams[owner] = func;
}

void Input::CloseInputStream(void* owner)
{
	auto it = _Instance->inputStreams.find(owner);
	if(it != _Instance->inputStreams.end()) {


		_Instance->inputStreams.erase(it);
	}
}

const float Input::GetInputAxis(string axisName, int controllerIdx) {
	if(axisName == "Horizontal" || axisName == "horizontal") {
		return ((float)Input::IsKeyHeld('D') - (float)Input::IsKeyHeld('A')) + LeftAxis(controllerIdx).x;
	}
	else {
		return ((float)Input::IsKeyHeld('W') - (float)Input::IsKeyHeld('S')) + LeftAxis(controllerIdx).y;
	}
}

const Vector2 Input::GetInputVector() {
	return Vector2(GetInputAxis("Horizontal"), GetInputAxis("Vertical"));
}

void Input::EndUpdate() {
	memcpy(&_Instance->prevKeyBuffer, &_Instance->keyBuffer, sizeof(bool) * KEYBUFF_SIZE); //Create duplicate of keybuffer for previous frame
	memcpy(&_Instance->prevmKeyBuffer, &_Instance->mKeyBuffer, sizeof(bool) * MKEYBUFF_SIZE);

	_Instance->prevMousePos = _Instance->mousePos;

	//_Instance->isMouseActive = false;
	if(_Instance->isMouseLocked) {
		SetCursorPos(static_cast<int>(ScreenCenter().x), static_cast<int>(ScreenCenter().y));
		//SetCursorPos(_Instance->newMouseClip.right / 2, _Instance->newMouseClip.bottom / 2);
	}
	//_Instance->isMouseActive = true;

	_Instance->inputAge++;

	_Instance->mouseDelta = XMFLOAT2(0, 0);

	UpdatePads();
}

void Input::Init(HWND hwnd) {
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
		//exit(70);
		MessageBox(0, L"Failed to register input devices", 0, 0);
	}

	GetMouseInformation();
	_Instance->prevMousePos = _Instance->mousePos; //Avoids a sudden perceived jump from 0,0 (default) to 1xxx,1xxx (actual position) at startup

	//CreateVectorMap("Horizontal", 'W', 'S', 'A', 'D');
}
