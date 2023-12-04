#include "TextInput.h"

#include "../Engine.h"

//void TextInput::AddListener(function<void()> func) {
//	_onClick.push_back(func);
//}

// takes keyboard key id and parses it as a key input
void TextInput::HandleKey(unsigned short key)
{
	//if(vKey >= 'A' && vKey <= 'Z') {
	//	bool isCaps = (GetKeyState(VK_CAPITAL) & 1) != 0;
	//	if(!isCaps) {
	//		vKey |= 32;
	//	}
	//}

	//if(vKey >= 32 && vKey <= 126) { // If its a normal character
	//if(
	//	vKey == ' ' ||
	//	(vKey >= '0' && vKey <= '9') ||
	//	(vKey >= 'A' && vKey <= 'Z') ||
	//	(vKey >= 'a' && vKey <= 'z')
	//) {
	//}
	if(key == 8) { // Backspace
		label->SetText(label->GetText().substr(0, label->GetText().size() - 1));
	}
	else {
		char ch = static_cast<char>(key);
		label->SetText(label->GetText() + ch);
	}
}

bool TextInput::IsPressed() {
	return isHovering && Input::IsMouseKeyHeld(MOUSE_L);
}

bool TextInput::IsHovering() {
	return isHovering;
}

bool TextInput::WasPressed() {
	return isHovering && Input::IsMouseKeyPressed(MOUSE_L);
}

void TextInput::SetText(const string& s) {
	label->SetText(s);
}

const string& TextInput::GetText() {
	return label->GetText();
}

void TextInput::SetTextColour(const float r, const float g, const float b, const float a) {
	label->SetColour(r, g, b, a);
}

const XMFLOAT4& TextInput::GetTextColour() {
	return label->GetColour();
}

void TextInput::SetBgColour(const float r, const float g, const float b, const float a) {
	rect->SetColour(r, g, b, a);
}

const XMFLOAT4& TextInput::GetBgColour() {
	return rect->GetColour();
}

void TextInput::SetLabel(const string& text, const wstring& fontPath, const XMFLOAT4& col) {
	if(label == nullptr) {
		label = new Label(fontPath, col);
		label->Init(Engine::Get()->GetGraphics()->GetDevice());
		label->InitSelf();
	}
	else {
		label->SetFontPath(fontPath);
		label->SetColour(col.x, col.y, col.z, col.w);
		label->SetText(label->GetText());
	}
	label->SetAnchor({ 0.0f, 0.5f });
	label->SetPivot({ 0.0f, 0.5f });
	label->SetParent(rect);
}

void TextInput::SetRect(const XMFLOAT4& bgCol)
{
	if(rect == nullptr) {
		rect = new UIRect("white", bgCol);
		rect->Init(Engine::Get()->GetGraphics()->GetDevice());
		rect->InitSelf();
		rect->SetPosition(this->rawPosition);
		rect->SetDimensions(this->dimensions);
	}
	else {
		rect->SetTexture("white");
		rect->SetColour(bgCol.x, bgCol.y, bgCol.z, bgCol.w);
		rect->SetPosition(this->rawPosition);
		rect->SetDimensions(this->dimensions);
	}
	rect->SetParent(this);
}

void TextInput::SetDimensions(const Vector2& dim) {
	this->dimensions = dim;

	rect->SetDimensions(dim);
}

//Button::Button() {
//	//InitSelf();
//	SetRect({1.f,1.f,1.f,1.f});
//	SetLabel("LABEL NOT SET", )
//}

TextInput::TextInput(const wstring& fontPath, const XMFLOAT4& col) {
	//InitSelf();
	if(rect == nullptr) SetRect({ 1.f,1.f,1.f,1.f });
	SetLabel("TEXT NOT SET", fontPath, col);
}

TextInput::TextInput(const XMFLOAT4& bgCol) {
	//InitSelf();
	SetRect(bgCol);
}

//Button::Button(const XMFLOAT4& txtCol, const XMFLOAT4& bgCol) {
//	//InitSelf();
//	SetRect(this->rawPosition, this->dimensions, { 1.f,1.f,1.f,1.f });
//	if(label==nullptr) SetLabel("TEXT NOT SET", )
//}

TextInput::TextInput(const string& text, const wstring& fontPath, const XMFLOAT4& txtCol, const XMFLOAT4& bgCol) {
	//InitSelf();
	SetRect(bgCol);
	SetLabel(text, fontPath, txtCol);
}

void TextInput::InitSelf() {}

void TextInput::Start() {}

void TextInput::Update(const float& dTime) {
	XMFLOAT2 mPos = Input::MousePosition();
	Vector2 sPos = GetScreenPosition();

	bool dbgKeyPress = Input::IsKeyPressed('L');


	isHovering =
		mPos.x < sPos.x + dimensions.x &&
		mPos.x > sPos.x &&
		mPos.y < sPos.y + dimensions.y &&
		mPos.y > sPos.y;

	if(Input::IsMouseKeyPressed(MOUSE_L)) {
		if(isHovering) {
			engaged = true;
			Input::OpenInputStream(this, [&](unsigned short kval) {
				// 8 is backspace
				//__nop();
				//label->SetText(label->GetText() + static_cast<char>(kval));

				this->HandleKey(kval);
				
			});
		}
		else {
			engaged = false;
			Input::CloseInputStream(this);
		}
	}

	//if(WasPressed()) {
	//	for(auto& func : _onClick) {
	//		func();
	//	}
	//}
}

void TextInput::Draw(SpriteBatch* spriteBatch) {
	rect->Draw(spriteBatch);
	label->Draw(spriteBatch);
}

TextInput::~TextInput() {
	if(label != nullptr) {
		delete label;
		label = nullptr;
	}

	if(rect != nullptr) {
		delete label;
		label = nullptr;
	}
}
