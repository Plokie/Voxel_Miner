#include "TitleScreen.h"

#include "../Engine/Engine.h"
#include "../Engine/UI/Button.h"
#include "../Engine/UI/TextInput.h"
#include "ChunkDatabase.h"
#include "WorldGen.h"
#include "PlayerData.h"

#include <fstream>
#include <tchar.h>
#include <string>
#include <locale>
#include <direct.h>

void GetSubdirectories(vector<wstring>* outVec) {
	struct stat info;
	stat("Worlds", &info); //get info on a Worlds folder

	if(!(info.st_mode & S_IFDIR)) { // if folder doesnt exit
		int ret = _mkdir("Worlds"); // create Worlds folder
	}

	WIN32_FIND_DATA findFileData;
	HANDLE hFind = FindFirstFile(L"Worlds\\*", &findFileData);

	if(hFind == INVALID_HANDLE_VALUE) {
		assert(false);
		return;
	}

	do {
		if(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if(_tcscmp(findFileData.cFileName, L".") != 0 &&
				_tcscmp(findFileData.cFileName, L"..") != 0) {
				
				wstring subDir = findFileData.cFileName;

				ifstream worldDat(L"Worlds\\" + subDir + L"\\world.dat");
				if(worldDat.good()) {
					outVec->push_back(subDir);
				}
			}
		}
	} while(FindNextFile(hFind, &findFileData) != 0);

	FindClose(hFind);
}

Button* AddButton(Engine* engine, Scene* scene, const string& text, const float y, function<void()> onClick) {
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

TextInput* AddTextInput(Engine* engine, Scene* scene, const string& text, const float y, const string& name/*, function<void()> onClick*/) {
	TextInput* txtIn = (TextInput*)scene->CreateObject2D(new TextInput("Baloo"), name);
	txtIn->SetAnchor(Vector2(0.5f, 0.5f));
	txtIn->SetDimensions(Vector2(500.f, 50.f));
	txtIn->SetPosition({ 0.f, y });
	txtIn->SetPivot({ 0.5f, 0.5f });
	txtIn->SetTemplateText(text);

	txtIn->rect->SetColour(0.1f, 0.1f, 0.1f, 1.0f);
	txtIn->label->SetColour(1.0f, 1.0f, 1.0f, 1.0f);

	//button->AddListener(onClick);

	return txtIn;
}

int parseSeedInput(const string& str) {
	if(str == "") {
		return rand() * ((rand() % 2) ? 1 : -1);
	}

	int returnValue = 0;
	
	for(auto it = str.rbegin(); it != str.rend(); it++) {
		const char& ch = *it;
		if(ch >= '0' && ch <= '9') { // This makes it so raw number inputs translate to the returnVal normally
			int retValLen = (int)((returnValue == 0) ? 0 : log10(returnValue) + 1);
			int charVal = ch - '0';
			returnValue += ((retValLen == 0) ? 1 : ((int)pow(10, retValLen))) * charVal;
		}
		else {
			returnValue += ch * (((returnValue % 2) == 0) ? -1 : 1);
		}
	}

	return returnValue;
}

bool validateWorldName(const string& worldName, string* out) {
	if(worldName == "" || worldName.size() == 0) return false;

	if(worldName.size() == 3 || worldName.size() == 4) {
		// Completely invalid folder names because OS reasons
		if(worldName.substr(0, 3) == "CON") return false; 
		if(worldName.substr(0, 3) == "COM") return false;
		if(worldName.substr(0, 3) == "LPT") return false;
		if(worldName.substr(0, 3) == "PRN") return false;
		if(worldName.substr(0, 3) == "AUX") return false;
		if(worldName.substr(0, 3) == "NUL") return false;
	}
	
	*out = "";
	out->reserve(worldName.size());

	for(char ch : worldName) {
		switch(ch) {
			//Invalid or possibly problematic characters
			case ' ': ch = '_'; break;
			case '.': ch = '_'; break;
			case '<': ch = '_'; break;
			case '>': ch = '_'; break;
			case ':': ch = '_'; break;
			case '"': ch = '_'; break;
			case '/': ch = '_'; break;
			case '\\':ch = '_'; break;
			case '|': ch = '_'; break;
			case '?': ch = '_'; break;
			case '*': ch = '_'; break;

			//Control characters
			case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 10: case 11: case 12: case 13: case 14: case 15: 
			case 16: case 17: case 18: case 19: case 20: case 21: case 22: case 23: case 24: case 25: case 26: case 27: case 28: case 29: case 30: case 31: 
				continue;
		}
		*out += ch;
	}

	if(*out == "" || out->size() == 0) return false;

	return true;
}

void TitleScreen::Setup(Engine* engine)
{
	Scene* titleMain = new Scene(Graphics::Get());
	titleMain->clearColor = { 0.2f, 0.2f, 0.2f, 1.f };

	AddButton(engine, titleMain, "Play", -150.f, [] {
		Engine::Get()->SetScene("titlePlay");
	});
	AddButton(engine, titleMain, "How to play", -50.f, [] {
		Engine::Get()->SetScene("howToPlay");
	});
	//AddButton(engine, titleMain, "Options", 50.f, [] {
	//	
	//});
	AddButton(engine, titleMain, "Quit", 150.f, [] {
		Engine::StopGame();
	});

	Label* creditLabel = new Label("Baloo");
	creditLabel->Init(Graphics::Get()->GetDevice());
	creditLabel->InitSelf();
	creditLabel->SetAnchor(0.f, 1.f);
	creditLabel->SetPivot(0.f, 1.f);
	creditLabel->SetText("Music: Undercover Vampire Policeman - Chris Zabriskie (CC4.0)");
	titleMain->CreateObject2D(creditLabel, "creditLabel");

	engine->AddScene(titleMain, "titleMain");

	//

	Scene* titlePlay = new Scene(Graphics::Get());
	titlePlay->clearColor = { 0.2f, 0.2f, 0.2f, 1.f };

	AddButton(engine, titlePlay, "New world", -100.f, [] {
		Engine::Get()->SetScene("titleNew");
	});
	AddButton(engine, titlePlay, "Load world", 0.f, [] {
		Engine::Get()->SetScene("titleLoad");
	});
	AddButton(engine, titlePlay, "Back", 100.f, [] {
		Engine::Get()->SetScene("titleMain");
	});

	engine->AddScene(titlePlay, "titlePlay");

	//

	Scene* howToPlay = new Scene(Graphics::Get());
	howToPlay->clearColor = { 0.2f, 0.2f, 0.2f, 1.f };

	Label* label1 = new Label("Baloo");
	label1->Init(Graphics::Get()->GetDevice());
	label1->InitSelf();
	label1->SetAnchor(0.f, 0.f);
	label1->SetPivot(0.f, 0.f);
	label1->SetText("Voxel Miner is a fundamental Minecraft clone. \n\
The world is a sandbox that you can build and destroy to any extent. \n\
While you play, you need to keep track of your health and hunger, and keep yourself fed and not die!\n\
You use your relevant tools to mine corresponding blocks to collect resources\n\
You can gain score by mining rarer ores, which you need better tools to mine!\n\
When you die, you drop all your items, your score is halved and you return to spawn.\n\
You can use the Workbench to craft items, and the Furnace to smelt items.\n\
Chests allow you to store items\n\
\n\
Controls:\n\
WASD to Move.   TAB to open Inventory.   Space to jump.    Shift to sprint, CTRL to walk slow\n\
Left click to mine, Right click to build/interact.    Esc to pause.    Q to drop held item\n\
Scroll wheel / number keys to change hotbar item.        \n\
Controller input is also supported. In UI, Left click is Right Trigger, Right click is Left Trigger\n\
");
	howToPlay->CreateObject2D(label1, "creditLabel");

	Button* button = (Button*)howToPlay->CreateObject2D(new Button("Baloo"), "howToPlayBack");
	button->SetAnchor(1.f, 1.f);
	button->SetDimensions(Vector2(500.f, 50.f));
	button->SetPosition({ 0.f, 0.f });
	button->SetPivot({ 1.f, 1.f });
	button->SetText("Back");

	button->rect->SetColour(0.3f, 0.3f, 0.3f, 1.0f);
	button->label->SetColour(1.0f, 1.0f, 1.0f, 1.0f);

	button->AddListener([] {Engine::Get()->SetScene("titleMain"); });

	engine->AddScene(howToPlay, "howToPlay");


	//

	Scene* titleNew = new Scene(Graphics::Get());
	titleNew->clearColor = { 0.2f, 0.2f, 0.2f, 1.f };

	TextInput* worldNameInput = AddTextInput(engine, titleNew, "World Name", -150.f, "name");
	TextInput* seedInput = AddTextInput(engine, titleNew, "Seed", -50.f, "seed");

	bool* currentGamemodeSelect = new bool(true); //hmm, should probably delete this but i cant think of a safe time to delete it
	//(its a single bool, itll probably be fine for now)
	//todo: make the rotating button select thing a class so its value gets deleted properly

	// Gamemode button
	Button* gamemodeButton = AddButton(engine, titleNew, "Gamemode: Survival", 50.f, [] {});
	gamemodeButton->AddListener([gamemodeButton, currentGamemodeSelect] {
		*currentGamemodeSelect = !*currentGamemodeSelect;
		if(*currentGamemodeSelect) {
			gamemodeButton->SetText("Gamemode: Survival");
		}
		else {
			gamemodeButton->SetText("Gamemode: Creative");
		}
		
		
	});

	// Create world button
	AddButton(engine, titleNew, "Create World", 150.f, [worldNameInput, seedInput, currentGamemodeSelect] {
		// Function call on click
		WorldGen::SetSeed(parseSeedInput(seedInput->label->GetText())); // Parse seed to integer
		string validWorldName = ""; // For out val
		if(!validateWorldName(worldNameInput->label->GetText(), &validWorldName)) 
			return; // If the name is completely unusable, return
		
		// Set the world name on the db and load
		ChunkDatabase::Get()->SetWorldName(validWorldName);
		Engine::Get()->SetScene("game");

		Engine::Get()->GetScene("game")->GetObject3D<PlayerData>("PlayerData")->SetGamemode((*currentGamemodeSelect) ? GM_SURVIVAL : GM_CREATIVE);
	});

	AddButton(engine, titleNew, "Back", 250.f, [] {
		Engine::Get()->SetScene("titlePlay");
	});

	engine->AddScene(titleNew, "titleNew");

	//

	Scene* titleLoad = new Scene(Graphics::Get());
	titleLoad->clearColor = { 0.2f, 0.2f, 0.2f, 1.f };

	//_findfirst
	
	vector<wstring> dirs;
	GetSubdirectories(&dirs);

	int i = 0;
	for(auto& ws : dirs) {
		//string s(ws.begin(), ws.end());
		string s;
		std::transform(ws.begin(), ws.end(), std::back_inserter(s), [](wchar_t c) {
			return (char)c;
		});

		AddButton(engine, titleLoad, s, -300.f + (100.f * i), [s] {
			ChunkDatabase::Get()->SetWorldName(s);
			Engine::Get()->SetScene("game");
		});

		i++;
	}

	AddButton(engine, titleLoad, "Back", 200.f, [] {
		Engine::Get()->SetScene("titlePlay");
	});

	engine->AddScene(titleLoad, "titleLoad");

}
