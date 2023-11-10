#pragma once

#include <SpriteBatch.h>
#include "MathUtil.h"
//#include "WinManager.h"

//fwd dec
class WinManager;

class Object2D {
protected:
	ID3D11Device* pDevice;
	WinManager* winMgr;
	// the pivot in pixels
	Vector2 pivot = { 0.0f, 0.0f };
	Vector2 anchor = { 0.0f, 0.0f };
	Vector2 dimensions;
	Vector2 rawPosition;
private:

public:
	float rotation;

	const Vector2& GetPivot();
	void SetPivot(const float& x, const float& y);
	void SetPivot(const Vector2& newPivot);

	const Vector2& GetAnchor();
	void SetAnchor(const float& x, const float& y);
	void SetAnchor(const Vector2& newAnchor);

	void SetPosition(const Vector2& position);
	void SetPosition(const float& x, const float& y);

	// Calculate raw screen position pased on pivot and anchor
	const Vector2& GetScreenPosition();

	void Init(ID3D11Device* pDevice);

	virtual void InitSelf();

	virtual void Start();
	virtual void Update(const float& dTime);
	virtual void Draw(SpriteBatch* spriteBatch);
};