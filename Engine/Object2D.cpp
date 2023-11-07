#include "Object2D.h"

const Vector2& Object2D::GetPivot()
{
    // TODO: insert return statement here
    return { 0,0 };
}

void Object2D::SetPivot(const float& x, const float& y)
{
    
}

void Object2D::SetPivot(const Vector2& newPivot)
{
    SetPivot(newPivot.x, newPivot.y);
}

void Object2D::Init(ID3D11Device* pDevice) {
    this->pDevice = pDevice;
}

void Object2D::InitSelf(){}

void Object2D::Start(){}

void Object2D::Update(const float& dTime){}

void Object2D::Draw(SpriteBatch* spriteBatch)
{
}
