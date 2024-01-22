#include "PauseScreen.h"

#include "../Engine/Engine.h"
#include "../Graphics/Graphics.h"
#include "../Engine/UI/Button.h"
#include "GameScene.h"
#include "ChunkDatabase.h"
#include "PlayerController.h"

Button* AddButton2(Engine* engine, Scene* scene, const string& text, const float y, function<void()> onClick) {
	Button* button = (Button*)scene->CreateObject2D(new Button("Baloo"), "button" + text);
	button->SetAnchor(Vector2(0.5f, 0.5f));
	button->SetDimensions(Vector2(500.f, 50.f));
	button->SetPosition({ 0.f, y });
	button->SetPivot({ 0.5f, 0.5f });
	button->SetText(text);

	button->rect->SetColour(0.3f, 0.3f, 0.3f, 1.0f);
	button->label->SetColour(1.0f, 1.0f, 1.0f, 1.0f);

	button->AddListener(onClick);
	return button;
}

void PauseScreen::Setup(Engine* engine)
{

	Scene* pauseScene = new Scene(Graphics::Get());
	pauseScene->clearColor = { 0.2f, 0.2f, 0.2f, 1.f };

	AddButton2(engine, pauseScene, "Resume", -50.f, [] {
		//Engine::Get()->SetScene("titlePlay");
		Close();
		Input::SetMouseLocked(true);
	});
	AddButton2(engine, pauseScene, "Exit to menu", 50.f, [engine] {
		//ChunkDatabase::Get()->Close();
		engine->SetScene("titleMain");
		Graphics::Get()->camera.transform.position = Vector3::Zero();
		engine->GetScene("game")->Clear(); // deleting chunkmanager closes the chunkdatabase
		GameScene::Setup(engine, engine->GetScene("game"));



		//ChunkDatabase::Get()->Close();
		//Engine::StopGame();
	});

	engine->AddScene(pauseScene, "pause");
}

void PauseScreen::Open()
{
	Engine* engine = Engine::Get();
	engine->SetScene("pause");
	Input::SetVirtualMouse(true);
}

void PauseScreen::Close()
{
	Engine* engine = Engine::Get();
	engine->SetScene("game");
	Input::SetVirtualMouse(false);
}
