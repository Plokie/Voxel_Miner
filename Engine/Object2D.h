#pragma once

#include "MathUtil.h"

class Object2D {
private:
	Vector2 textureSize;
public:
	Vector2 position;
	float rotation;

	Vector2 anchor = { 0.f ,0.f };

	// the pivot in pixels
	Vector2 pivot = { 0.0f, 0.0f };

	Vector2 GetPivot();
	Vector2 GetRelativePivot();
	void SetPivot();
	void SetRelativePivot();
};