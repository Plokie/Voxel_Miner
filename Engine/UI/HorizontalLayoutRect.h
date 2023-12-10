#pragma once

#include <vector>
#include "UIRect.h"

class HorizontalLayoutRect : public UIRect {
private:
	vector<Object2D*> children = {};

	Vector2 childAnchor = {0.5f, 0.5f};
	Vector2 padding = { 5.f, 5.f };
	float spacing = 5.f;

public:
	HorizontalLayoutRect();
	HorizontalLayoutRect(const XMFLOAT4& col);

	void SetChildAnchor(const Vector2& a);

	void AddChild(Object2D* c);
	const vector<Object2D*>& GetChildren() const;

	void RealignChildren();
};