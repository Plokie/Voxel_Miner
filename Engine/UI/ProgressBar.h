#pragma once

#include "UIRect.h"

enum PG_DIR : unsigned char { LEFT_TO_RIGHT, RIGHT_TO_LEFT };

class ProgressBar : public Object2D {
private:
	UIRect* progress = nullptr;
	UIRect* background = nullptr;


	PG_DIR dir = LEFT_TO_RIGHT;
	float _value = 1.f;

	void UpdateProgressbar();
	void ReanchorProgress();
public:

	ProgressBar();
	~ProgressBar();

	void SetDirection(PG_DIR dir);

	void SetValue(float val);
	void ChangeValue(float delta);
	const float GetValue() const { return _value; }

	void SetProgressCol(const XMFLOAT4& col);
	void SetBgCol(const XMFLOAT4& col);

	void InitSelf() override;

	void SetDimensions(const Vector2& dim) override;

	void Start() override;
	void Update(const float dt) override;
	void Draw(SpriteBatch* spriteBatch) override;
};