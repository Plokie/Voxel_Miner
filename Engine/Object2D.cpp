#include "Object2D.h"

#include "WinManager.h"
#include "MathUtil.h"

void Object2D::SetEnabled(const bool e) {
    this->enabled = e;
}

const bool Object2D::GetEnabled() const {
    return enabled;
}

bool Object2D::DoDraw() {
    if(parent != nullptr) {
        return parent->DoDraw();
    }

    return enabled;
}

void Object2D::SetParent(Object2D* parent)
{
    this->parent = parent;
}

Object2D* Object2D::GetParent() const
{
    return parent;
}

void Object2D::SetDepth(const float depth) {
    this->depth = depth;
}

const float Object2D::GetDepth() const {
    return depth;
}

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

const Vector2& Object2D::GetDimensions() const {
    return dimensions;
}

void Object2D::SetDimensions(const Vector2& dim) {
    this->dimensions = dim;
}

void Object2D::SetPosition(const Vector2& position)
{
    this->rawPosition = position;
}

void Object2D::SetPosition(const float& x, const float& y)
{
    SetPosition({ x,y });
}

const Vector2 Object2D::GetScreenPosition()
{
    Vector2 res = { (float)winMgr->width, (float)winMgr->height};

    Vector2 parentPosition = { 0.f, 0.f };
    if (parent != nullptr) {
        parentPosition = parent->GetScreenPosition();
        //if (parent->dimensions != Vector2(0.f, 0.f)) {
            res = parent->dimensions;
        //}
    }

    return parentPosition + (this->rawPosition + (res ^ anchor) - (pivot ^ dimensions));
}

void Object2D::Init(ID3D11Device* pDevice) {
    this->pDevice = pDevice;
    this->winMgr = WinManager::Get();
}

void Object2D::InitSelf(){}

void Object2D::Start(){}

void Object2D::Update(const float dTime){}

void Object2D::Draw(SpriteBatch* spriteBatch) {}
