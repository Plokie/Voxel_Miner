#include "Object2D.h"

#include "WinManager.h"

const Vector2& Object2D::GetPivot()
{
    return this->pivot;
}

void Object2D::SetPivot(const float& x, const float& y)
{
    SetPivot({ x, y });
}

void Object2D::SetPivot(const Vector2& newPivot)
{
    this->pivot = newPivot;
}

const Vector2& Object2D::GetAnchor()
{
    // TODO: insert return statement here
    return this->anchor;
}

void Object2D::SetAnchor(const float& x, const float& y)
{
    SetAnchor({ x, y });
}

void Object2D::SetAnchor(const Vector2& newAnchor)
{
    this->anchor = newAnchor;
}

void Object2D::SetPosition(const Vector2& position)
{
    this->rawPosition = position;
}

void Object2D::SetPosition(const float& x, const float& y)
{
    SetPosition({ x,y });
}

const Vector2& Object2D::GetScreenPosition()
{
    // TODO: insert return statement here
    return this->rawPosition;
}

void Object2D::Init(ID3D11Device* pDevice) {
    this->pDevice = pDevice;
    this->winMgr = WinManager::Get();
}

void Object2D::InitSelf(){}

void Object2D::Start(){}

void Object2D::Update(const float& dTime){}

void Object2D::Draw(SpriteBatch* spriteBatch)
{
}
