#include "TitleScreen.h"

#include "../Engine/Engine.h"

void TitleScreen::Start()
{
}

void TitleScreen::Update(float dt)
{
	if(Input::IsKeyPressed(VK_SPACE)) {
		Engine::Get()->SetScene("game");
	}
}
