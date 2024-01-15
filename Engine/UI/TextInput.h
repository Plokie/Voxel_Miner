#pragma once

#include <vector>
#include "Label.h"
#include "UIRect.h"

using namespace std;

class TextInput : public Object2D {
private:

	bool isHovering;
	bool engaged;

	vector<function<void()>> _events = {};
	void InvokeEvents();

public:
	Label* label = nullptr;
	Label* templateLabel = nullptr;
	UIRect* rect = nullptr;
	UIRect* cursor = nullptr;

	void AddListener(function<void()> func);
	void HandleKey(unsigned short vKey);

	bool IsPressed();
	bool IsHovering();
	bool WasPressed();

	const bool IsEngaged() const { return engaged; }

	void SetText(const string& s);
	const string& GetText();

	void SetTemplateText(const string& s);
	void SetTemplateTextColour(const float r, const float g, const float b, const float a);

	void SetTextColour(const float r, const float g, const float b, const float a);
	const XMFLOAT4& GetTextColour();

	void SetBgColour(const float r, const float g, const float b, const float a);
	const XMFLOAT4& GetBgColour();

	void SetLabel(const string& text, const string& font, const XMFLOAT4& col);
	void SetTemplateLabel(const string& text, const string& font, const XMFLOAT4& col);
	void SetRect(const XMFLOAT4& bgCol);
	void SetCursor(const XMFLOAT4& bgCol);

	void SetDimensions(const Vector2& dim) override;

	//Button();
	//Button(const string& text, const wstring& fontPath, const XMFLOAT4& col = { 0.f,0.f,0.f,1.f });
	TextInput(const string& font, const XMFLOAT4& col = { 0.f,0.f,0.f,1.f }, const XMFLOAT4& templateCol = { 0.3f,0.3f,0.3f,1.f });
	TextInput(const XMFLOAT4& bgCol);
	//Button(const XMFLOAT4& txtCol, const XMFLOAT4& bgCol);
	TextInput(const string& text, const string& font, const XMFLOAT4& txtCol, const XMFLOAT4& bgCol);

	void InitSelf() override;

	void Start() override;
	void Update(const float dTime) override;
	void Draw(SpriteBatch* spriteBatch) override;

	~TextInput();
};