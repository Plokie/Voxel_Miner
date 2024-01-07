#pragma once

#include "UIRect.h"
#include "Label.h"
#include <functional>
#include <vector>

class Button : public Object2D {
private:
	bool isHovering;
	
	vector<function<void()>> _onClick = {};

public:
	Label* label = nullptr;
	UIRect* rect = nullptr;

	void AddListener(function<void()> func);

	bool IsPressed();
	bool IsHovering();
	bool WasPressed();

	void SetText(const string& s);
	const string& GetText();

	void SetTextColour(const float r, const float g, const float b, const float a);
	const XMFLOAT4& GetTextColour();

	void SetBgColour(const float r, const float g, const float b, const float a);
	const XMFLOAT4& GetBgColour();

	void SetLabel(const string& text, const string& font, const XMFLOAT4& col);
	void SetRect(const XMFLOAT4& bgCol);

	void SetDimensions(const Vector2& dim) override;

	//Button();
	Button(const string& font, const XMFLOAT4& txtCol = { 0.f,0.f,0.f,1.f });
	Button(const XMFLOAT4& bgCol);
	//Button(const XMFLOAT4& txtCol, const XMFLOAT4& bgCol);
	Button(const string& text, const string& font, const XMFLOAT4& txtCol, const XMFLOAT4& bgCol);

	void InitSelf() override;

	void Start() override;
	void Update(const float dTime) override;
	void Draw(SpriteBatch* spriteBatch) override;

	~Button();
};