#pragma once

#include <string>
#include <SpriteFont.h>
#include "../Object2D.h"

class Label : public Object2D {
private:
	//Data\\Fonts\\Baloo2.spritefont
	string font = "Baloo";
	SpriteFont* spriteFont = nullptr;
	string text = "NO TEXT SET";
	XMFLOAT4 color = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
public:

	void SetColour(const float& r, const float& g, const float& b, const float& a);
	const XMFLOAT4& GetColour();

	void SetText(const string& text);
	const string& GetText();

	void SetFont(const string& font);
	const SpriteFont* GetSpriteFont() { return spriteFont; }

	//Label(const wstring& fontPath, const string& text, const XMFLOAT4& = XMFLOAT4(1.f, 1.f, 1.f, 1.f));
	Label(const string& font, const XMFLOAT4& = XMFLOAT4(1.f, 1.f, 1.f, 1.f));
	//~Label();

	void InitSelf() override;
	void Start() override;
	void Update(const float dTime) override;
	void Draw(SpriteBatch* spriteBatch) override;
};