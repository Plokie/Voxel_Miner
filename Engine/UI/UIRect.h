#pragma once

#include "../Object2D.h"

class UIRect : public Object2D {
private:
	XMFLOAT4 color = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	//Vector2 dimensions; // Object dimensions (width and height)
	ID3D11ShaderResourceView* tex = nullptr;

public:
	void SetTexture(const string& texName);
	void SetTexture(ID3D11ShaderResourceView* tex);

	void SetColour(const float& r, const float& g, const float& b, const float& a);
	const XMFLOAT4 GetColour();

	UIRect();
	UIRect(const XMFLOAT4& col);
	UIRect(const string& texName, const XMFLOAT4& col = XMFLOAT4(1.f, 1.f, 1.f, 1.f));

	void InitSelf() override;
	void Start() override;
	void Update(const float dTime) override;
	void Draw(SpriteBatch* spriteBatch) override;
};