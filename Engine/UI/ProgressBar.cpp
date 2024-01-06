#include "ProgressBar.h"

#include "../Engine.h"

void ProgressBar::UpdateProgressbar() {
	progress->SetDimensions({ this->dimensions.x * _value, this->dimensions.y });

	//switch(dir) { //might need in future for TOP_TO_BOTTOM and BOTTOM_TO_TOP
	//case LEFT_TO_RIGHT:
	//	break;

	//case RIGHT_TO_LEFT:
	//	progress->SetDimensions({ this->dimensions.x * _value, this->dimensions.y });
	//	break;
	//}

	
}

void ProgressBar::ReanchorProgress() {
	switch(dir) {
	case LEFT_TO_RIGHT:
		progress->SetPivot(0.f, 0.5f);
		progress->SetAnchor(0.f, 0.5f);
		break;

	case RIGHT_TO_LEFT:
		progress->SetPivot(1.f, 0.5f);
		progress->SetAnchor(1.f, 0.5f);
		break;
	}
}

ProgressBar::ProgressBar() {
	SetBgCol({0.f, 0.f, 0.f, 1.f});
	SetProgressCol({1.f, 1.f, 1.f, 1.f});
}

ProgressBar::~ProgressBar() {
	if(progress) delete progress;
	if(background) delete background;
}

void ProgressBar::SetDirection(PG_DIR dir) {
	this->dir = dir;
	ReanchorProgress();
	UpdateProgressbar();
}

void ProgressBar::SetValue(float val)
{
	_value = val;
	UpdateProgressbar();
}

void ProgressBar::ChangeValue(float delta)
{
	_value += delta;
	UpdateProgressbar();
}

void ProgressBar::SetProgressCol(const XMFLOAT4& col) {
	if(!progress) {
		progress = new UIRect("white", col);
		progress->Init(Graphics::Get()->GetDevice());
		progress->InitSelf();
	}
	else {
		progress->SetColour(col.x, col.y, col.z, col.w);
	}

	if(!background) {
		SetBgCol({ 0.f, 0.f, 0.f, 1.0f });
	}

	progress->SetParent(background);
	ReanchorProgress();
	UpdateProgressbar();
}

void ProgressBar::SetBgCol(const XMFLOAT4& col) {
	if(!background) {
		background = new UIRect("white", col);
		background->Init(Graphics::Get()->GetDevice());
		background->InitSelf();
	}
	else {
		background->SetColour(col.x, col.y, col.z, col.w);
	}

	background->SetParent(this);
	//background->set
}

void ProgressBar::InitSelf() {
}

void ProgressBar::SetDimensions(const Vector2& dim)
{
	this->dimensions = dim;
	background->SetDimensions(dim);
	UpdateProgressbar();
}

void ProgressBar::Start()
{
}

void ProgressBar::Update(const float dt)
{
}

void ProgressBar::Draw(SpriteBatch* spriteBatch) {
	background->Draw(spriteBatch);
	progress->Draw(spriteBatch);
}
