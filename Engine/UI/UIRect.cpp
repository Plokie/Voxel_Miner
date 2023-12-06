#include "UIRect.h"

#include "../../Graphics/Resources.h"
#include <SimpleMath.h>

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

void UIRect::SetTexRect(RECT r) {
	hasTexRect = !(r.left == 0L && r.top == 0L && r.right == 0L && r.bottom == 0L);
	texRect = r;
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

void UIRect::Update(const float dTime)
{
}

void UIRect::Draw(SpriteBatch* spriteBatch)
{
	if (tex == nullptr) return;
	//spriteBatch->Draw(tex, GetScreenPosition(), { 0.f, 0.f, dimensions.x, dimensions.y }  );
	//spriteBatch->Draw(tex, GetScreenPosition(), );
	const Vector2 screenPos = GetScreenPosition();
	const RECT r = { static_cast<LONG>(screenPos.x), static_cast<LONG>(screenPos.y), static_cast<LONG>(screenPos.x + dimensions.x), static_cast<LONG>(screenPos.y + dimensions.y) };
	//const RECT sr = { 0.f, 0.f, 0.f, 0.f };

	if(hasTexRect) {
		spriteBatch->Draw(tex, r, &texRect, { color.x, color.y, color.z, color.w });
	}
	else {
		spriteBatch->Draw(tex, r, { color.x, color.y, color.z, color.w });
	}
	//spriteBatch->Draw(tex, DirectX::SimpleMath::Vector2( screenPos.x, screenPos.y ), texRect, DirectX::SimpleMath::Vector4( color.x,color.y,color.z,color.w ), rotation, DirectX::SimpleMath::Vector2(0.f, 0.f), DirectX::SpriteEffects_None);
}
