#include "HorizontalLayoutRect.h"

HorizontalLayoutRect::HorizontalLayoutRect()
{
	this->SetColour(1.f,1.f,1.f,1.f);
}

HorizontalLayoutRect::HorizontalLayoutRect(const XMFLOAT4& col)
{
	this->SetColour(col.x, col.y, col.z, col.w);
}

void HorizontalLayoutRect::SetChildAnchor(const Vector2& a) {
	childAnchor = a;
}

void HorizontalLayoutRect::AddChild(Object2D* c) {
	children.push_back(c);

	c->SetParent(this);

	RealignChildren();
}

const vector<Object2D*>& HorizontalLayoutRect::GetChildren() const {
	return children;
}

void HorizontalLayoutRect::RealignChildren() {
	int i = 0;
	float totalOffset = padding.x;
	for(auto it = children.begin(); it != children.end(); it++) {
		Object2D*& c = *it;
		if(c == nullptr) children.erase(it);

		c->SetParent(this);
		//c->SetAnchor(childAnchor);

		//todo: use child anchor
		c->SetAnchor({0.0f, 0.5f});
		c->SetPivot({0.0f, 0.5f});

		c->SetPosition({totalOffset, 0});

		totalOffset += c->GetDimensions().x + spacing;
	}
}

