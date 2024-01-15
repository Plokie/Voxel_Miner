#include "Button.h"

#include "../Engine.h"

void Button::AddListener(function<void()> func){
	_onClick.push_back(func);
}

bool Button::IsPressed() {
	return isHovering && Input::IsMouseKeyHeld(MOUSE_L);
}

bool Button::IsHovering() {
	return isHovering;
}

bool Button::WasPressed() {
	return isHovering && Input::IsMouseKeyPressed(MOUSE_L);
}

void Button::SetText(const string& s) {
	label->SetText(s);
}

const string& Button::GetText() {
	return label->GetText();
}

void Button::SetTextColour(const float r, const float g, const float b, const float a) {
	label->SetColour(r, g, b, a);
}

const XMFLOAT4& Button::GetTextColour() {
	return label->GetColour();
}

void Button::SetBgColour(const float r, const float g, const float b, const float a) {
	rect->SetColour(r, g, b, a);
}

const XMFLOAT4& Button::GetBgColour() {
	return rect->GetColour();
}

void Button::SetLabel(const string& text, const string& font, const XMFLOAT4& col) {
	if(label == nullptr) {
		label = new Label(font, col);
		label->Init(Engine::Get()->GetGraphics()->GetDevice());
		label->InitSelf();
	}
	else {
		label->SetFont(font);
		label->SetColour(col.x, col.y, col.z, col.w);
		label->SetText(label->GetText());
	}
	label->SetAnchor({ 0.5f, 0.5f });
	label->SetPivot({ 0.5f, 0.5f });
	label->SetParent(rect);
}

void Button::SetRect(const XMFLOAT4& bgCol)
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

void Button::SetDimensions(const Vector2& dim) {
	this->dimensions = dim;

	rect->SetDimensions(dim);
}

//Button::Button() {
//	//InitSelf();
//	SetRect({1.f,1.f,1.f,1.f});
//	SetLabel("LABEL NOT SET", )
//}

Button::Button(const string& font, const XMFLOAT4& col) {
	//InitSelf();
	if(rect == nullptr) SetRect({ 1.f,1.f,1.f,1.f });
	SetLabel("", font, col);
}

Button::Button(const XMFLOAT4& bgCol) {
	//InitSelf();
	SetRect(bgCol);
}

//Button::Button(const XMFLOAT4& txtCol, const XMFLOAT4& bgCol) {
//	//InitSelf();
//	SetRect(this->rawPosition, this->dimensions, { 1.f,1.f,1.f,1.f });
//	if(label==nullptr) SetLabel("TEXT NOT SET", )
//}

Button::Button(const string& text, const string& font, const XMFLOAT4& txtCol, const XMFLOAT4& bgCol) {
	//InitSelf();
	SetRect(bgCol);
	SetLabel(text, font, txtCol);
}

void Button::InitSelf() {}

void Button::Start() {}

void Button::Update(const float dTime) {
	XMFLOAT2 mPos = Input::MousePosition();
	Vector2 sPos = GetScreenPosition();

	isHovering =
		mPos.x < sPos.x + dimensions.x &&
		mPos.x > sPos.x &&
		mPos.y < sPos.y + dimensions.y &&
		mPos.y > sPos.y;

	if(WasPressed()) {
		for(auto& func : _onClick) {
			func();
		}
	}
}

void Button::Draw(SpriteBatch* spriteBatch) {
	rect->Draw(spriteBatch);
	if(label!=nullptr)
	label->Draw(spriteBatch);
}

Button::~Button() {
	if(label != nullptr) {
		delete label;
		label = nullptr;
	}

	if(rect != nullptr) {
		delete label;
		label = nullptr;
	}
}
