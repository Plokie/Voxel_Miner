#include "UIRect.h"

#include "../../Graphics/Resources.h"

void UIRect::SetTexture(const string& texName)
{
	ID3D11ShaderResourceView* tex = Resources::GetTexture(texName);
	if (tex) {
		this->tex = tex;
	}
	else assert(false); // Tex was nullptr
}

void UIRect::SetTexture(ID3D11ShaderResourceView* tex)
{
	this->tex = tex;
}

void UIRect::SetColour(const float& r, const float& g, const float& b, const float& a)
{
	this->color = { r,g,b,a };
}

const XMFLOAT4 UIRect::GetColour()
{
	return XMFLOAT4();
}

UIRect::UIRect()
{
	SetTexture("white"); // We have to assume white is loaded
}

UIRect::UIRect(const XMFLOAT4& col)
{
	this->color = col;
}

UIRect::UIRect(const string& texName, const XMFLOAT4& col)
{
	this->color = col;
	SetTexture(texName);
}

void UIRect::InitSelf()
{
}

void UIRect::Start()
{
}

void UIRect::Update(const float& dTime)
{
}

void UIRect::Draw(SpriteBatch* spriteBatch)
{
	if (tex == nullptr) return;
	//spriteBatch->Draw(tex, GetScreenPosition(), { 0.f, 0.f, dimensions.x, dimensions.y }  );
	//spriteBatch->Draw(tex, GetScreenPosition(), );
	const Vector2 screenPos = GetScreenPosition();
	const RECT r = { screenPos.x, screenPos.y, screenPos.x + dimensions.x, screenPos.y + dimensions.y };

	spriteBatch->Draw(tex, r);
}
