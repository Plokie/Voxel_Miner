#pragma once

#include <SpriteBatch.h>
#include "MathUtil.h"

class Object2D {
protected:
	ID3D11Device* pDevice;
	// the pivot in pixels
	Vector2 pivot = { 0.0f, 0.0f };
	Vector2 dimensions;
private:

public:
	Vector2 position;
	float rotation;

	Vector2 anchor = { 0.f ,0.f };

	const Vector2& GetPivot();
	void SetPivot(const float& x, const float& y);
	void SetPivot(const Vector2& newPivot);

	void Init(ID3D11Device* pDevice);

	virtual void InitSelf();

	virtual void Start();
	virtual void Update(const float& dTime);
	virtual void Draw(SpriteBatch* spriteBatch);
};