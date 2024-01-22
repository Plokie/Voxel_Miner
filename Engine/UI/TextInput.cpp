#include "TextInput.h"

#include "../Engine.h"

//void TextInput::AddListener(function<void()> func) {
//	_onClick.push_back(func);
//}

void TextInput::InvokeEvents()
{
	for(auto& event : _events) {
		event();
	}
}

void TextInput::AddListener(function<void()> func) {
	_events.push_back(func);
}

// takes keyboard key id and parses it as a key input
void TextInput::HandleKey(unsigned short key)
{
	if(key == 8) { // Backspace
		label->SetText(label->GetText().substr(0, label->GetText().size() - 1));
	}
	else {
		char ch = static_cast<char>(key);
		if(label->GetSpriteFont()->ContainsCharacter(ch)) {
			label->SetText(label->GetText() + ch);
		}
	}

	InvokeEvents();
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

void TextInput::SetTemplateText(const string& s)
{
	templateLabel->SetText(s);
}

void TextInput::SetTemplateTextColour(const float r, const float g, const float b, const float a)
{
	templateLabel->SetColour(r, g, b, a);
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

void TextInput::SetLabel(const string& text, const string& font, const XMFLOAT4& col) {
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
	label->SetAnchor({ 0.0f, 0.5f });
	label->SetPivot({ 0.0f, 0.5f });
	label->SetParent(rect);
}

void TextInput::SetTemplateLabel(const string& text, const string& font, const XMFLOAT4& col)
{
	if(templateLabel == nullptr) {
		templateLabel = new Label(font, col);
		templateLabel->Init(Engine::Get()->GetGraphics()->GetDevice());
		templateLabel->InitSelf();
	}
	else {
		templateLabel->SetFont(font);
		templateLabel->SetColour(col.x, col.y, col.z, col.w);
		templateLabel->SetText(templateLabel->GetText());
	}
	templateLabel->SetAnchor({ 0.0f, 0.5f });
	templateLabel->SetPivot({ 0.0f, 0.5f });
	templateLabel->SetParent(rect);
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

void TextInput::SetCursor(const XMFLOAT4& bgCol)
{
	if(cursor == nullptr) {
		cursor = new UIRect("white", bgCol);
		cursor->Init(Engine::Get()->GetGraphics()->GetDevice());
		cursor->InitSelf();
		cursor->SetPosition({ 5.f, 0.f });
		cursor->SetDimensions({5.f, 30.f});
	}
	else {
		cursor->SetTexture("white");
		cursor->SetColour(bgCol.x, bgCol.y, bgCol.z, bgCol.w);
		cursor->SetPosition({5.f, 0.f});
		cursor->SetDimensions({ 5.f, 30.f });
	}
	cursor->SetParent(label);
	cursor->SetAnchor({ 1.0f, 0.5f });
	cursor->SetPivot({ 0.5f, 0.5f });
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

TextInput::TextInput(const string& font, const XMFLOAT4& col, const XMFLOAT4& templateCol) {
	//InitSelf();
	if(rect == nullptr) SetRect({ 1.f,1.f,1.f,1.f });
	SetLabel("", font, col);
	SetTemplateLabel("TEMPLATE NOT SET", font, templateCol);
	SetCursor({ 1.f,1.f,1.f,1.f });
}

TextInput::TextInput(const XMFLOAT4& bgCol) {
	//InitSelf();
	SetRect(bgCol);
	//SetTemplateLabel("TEMPLATE NOT SET", font, col);
	SetCursor({ 1.f,1.f,1.f,1.f });
}

//Button::Button(const XMFLOAT4& txtCol, const XMFLOAT4& bgCol) {
//	//InitSelf();
//	SetRect(this->rawPosition, this->dimensions, { 1.f,1.f,1.f,1.f });
//	if(label==nullptr) SetLabel("TEXT NOT SET", )
//}

TextInput::TextInput(const string& text, const string& font, const XMFLOAT4& txtCol, const XMFLOAT4& bgCol) {
	//InitSelf();
	SetRect(bgCol);
	SetLabel("", font, txtCol);
	SetTemplateLabel(text, font, { .3f,.3f,.3f,1.f});
	SetCursor({ 1.f,1.f,1.f,1.f });
}

void TextInput::InitSelf() {}

void TextInput::Start() {}

void TextInput::Update(const float dTime) {
	XMFLOAT2 mPos = Input::MousePosition();
	Vector2 sPos = GetScreenPosition();

	isHovering =
		mPos.x < sPos.x + dimensions.x &&
		mPos.x > sPos.x &&
		mPos.y < sPos.y + dimensions.y &&
		mPos.y > sPos.y;

	if(Input::IsMouseKeyPressed(MOUSE_L)) {
		if(isHovering) {
			engaged = true;
			Input::OpenInputStream(this, [&](unsigned short kval) {
				this->HandleKey(kval);
			});
		}
		else {
			engaged = false;
			Input::CloseInputStream(this);
		}
	}
}

void TextInput::Draw(SpriteBatch* spriteBatch) {
	rect->Draw(spriteBatch);
	label->Draw(spriteBatch);
	if(engaged)
		cursor->Draw(spriteBatch);
	else if(label->GetText() == "")
		templateLabel->Draw(spriteBatch);
}

TextInput::~TextInput() {
	Input::CloseInputStream(this);

	if(label != nullptr) {
		delete label;
		label = nullptr;
	}

	if(templateLabel != nullptr) {
		delete templateLabel;
		templateLabel = nullptr;
	}

	if(rect != nullptr) {
		delete rect;
		rect = nullptr;
	}

	if(cursor != nullptr) {
		delete cursor;
		cursor = nullptr;
	}
}
