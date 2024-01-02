#pragma once

#include <SpriteBatch.h>
#include "MathUtil.h"
//#include "WinManager.h"

//fwd dec
class WinManager;

class Object2D {
protected:
	Object2D* parent = nullptr;
	ID3D11Device* pDevice = nullptr;
	WinManager* winMgr = nullptr;
	// the pivot in pixels
	Vector2 pivot = { 0.0f, 0.0f };
	Vector2 anchor = { 0.0f, 0.0f };
	Vector2 dimensions = { 0.f, 0.f };
	Vector2 rawPosition;

	

	bool enabled = true;
	float depth = 0.0f;
private:

public:
	void SetEnabled(const bool e);
	const bool GetEnabled() const;

	bool DoDraw();

	void SetParent(Object2D* parent);
	Object2D* GetParent() const;

	void SetDepth(const float depth);
	const float GetDepth() const;

	// Gets the pivot (stored as pixels)
	const Vector2& GetPivot();
	// Sets the pivot (stored as pixels)
	void SetPivot(const float& x, const float& y);
	// Sets the pivot (stored as ratio)
	void SetPivot(const Vector2& newPivot);

	// Gets the anchor (stored as ratio)
	const Vector2& GetAnchor();
	// Sets the anchor (stored as ratio)
	void SetAnchor(const float& x, const float& y);
	// Sets the anchor (stored as ratio)
	void SetAnchor(const Vector2& newAnchor);


	const Vector2& GetDimensions() const;
	virtual void SetDimensions(const Vector2& dim);

	void SetPosition(const Vector2& position);
	void SetPosition(const float& x, const float& y);

	// Calculate raw screen position pased on pivot and anchor
	virtual const Vector2 GetScreenPosition();

	void Init(ID3D11Device* pDevice);

	virtual void InitSelf();

	virtual void Start();
	virtual void Update(const float dTime);
	virtual void Draw(SpriteBatch* spriteBatch);
};