#include "Label.h"

void Label::InitSelf()
{
	spriteFont = new SpriteFont(pDevice, this->fontPath.c_str());
}

void Label::SetColour(const float& r, const float& g, const float& b, const float& a)
{
	this->color = XMFLOAT4(r, g, b, a);
}

const XMFLOAT4 Label::GetColour()
{
	return this->color;
}

void Label::SetText(const string& text)
{
	this->text = text;
}

const string& Label::GetText()
{
	return this->text;
}

void Label::SetFontPath(const wstring& fontPath)
{
	this->fontPath = fontPath;
}

Label::Label(const wstring& fontPath, const string& text, const XMFLOAT4& color)
{
	this->fontPath = fontPath;
	this->text = text;
	this->color = color;
}

void Label::Start()
{
}

void Label::Update(const float& dTime)
{
}

void Label::Draw(SpriteBatch* spriteBatch)
{
	this->spriteFont->DrawString(spriteBatch, this->text.c_str(), XMFLOAT2(this->rawPosition.x, this->rawPosition.y), XMVectorSet(color.x,color.y,color.z,color.w));
}
