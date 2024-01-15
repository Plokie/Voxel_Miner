#include "Label.h"

#include "../../Graphics/Resources.h"

void Label::InitSelf()
{
	//spriteFont = new SpriteFont(pDevice, this->fontPath.c_str());
	spriteFont = Resources::GetFont(this->font);
}

void Label::SetColour(const float& r, const float& g, const float& b, const float& a)
{
	this->color = XMFLOAT4(r, g, b, a);
}

const XMFLOAT4& Label::GetColour()
{
	return this->color;
}

void Label::SetText(const string& text)
{
	this->text = text;

	Vector2 dim = this->spriteFont->MeasureString(text.c_str());
	dimensions = dim;
}

const string& Label::GetText()
{
	return this->text;
}

void Label::SetFont(const string& font)
{
	this->font = font;
	spriteFont = Resources::GetFont(this->font);
}
//
//Label::Label(const wstring& fontPath, const string& text, const XMFLOAT4& color)
//{
//	this->fontPath = fontPath;
//	this->text = text;
//	this->color = color;
//}

Label::Label(const string& font, const XMFLOAT4& color)
{
	this->font = font;
	this->text = "";
	this->color = color;
}

//Label::~Label()
//{
//	//if(spriteFont) delete spriteFont; //todo: spriteFont be part of Resources class
//}

void Label::Start()
{
}

void Label::Update(const float dTime)
{
}

void Label::Draw(SpriteBatch* spriteBatch)
{
	this->spriteFont->DrawString(spriteBatch, this->text.c_str(), GetScreenPosition(), XMVectorSet(color.x, color.y, color.z, color.w));
}
