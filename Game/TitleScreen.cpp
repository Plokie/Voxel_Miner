#include "TitleScreen.h"

#include "../Engine/Engine.h"
#include "../Engine/UI/Button.h"
#include "../Engine/UI/TextInput.h"
#include "ChunkDatabase.h"
#include "WorldGen.h"

#include <fstream>
#include <tchar.h>
#include <string>
#include <locale>

void GetSubdirectories(vector<wstring>* outVec) {
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
	txtIn->SetText(text);

	txtIn->rect->SetColour(0.1f, 0.1f, 0.1f, 1.0f);
	txtIn->label->SetColour(1.0f, 1.0f, 1.0f, 1.0f);

	//button->AddListener(onClick);

	return txtIn;
}

int parseSeedInput(const string& str) {
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

	});
	AddButton(engine, titleMain, "Options", 50.f, [] {
		
	});
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

	Scene* titleNew = new Scene(Graphics::Get());
	titleNew->clearColor = { 0.2f, 0.2f, 0.2f, 1.f };

	TextInput* worldNameInput = AddTextInput(engine, titleNew, "World Name", -150.f, "name");
	TextInput* seedInput = AddTextInput(engine, titleNew, "1337", -50.f, "seed");

	// Create world button
	AddButton(engine, titleNew, "Create", 50.f, [worldNameInput, seedInput] {
		// Function call on click
		WorldGen::SetSeed(parseSeedInput(seedInput->label->GetText())); // Parse seed to integer
		string validWorldName = ""; // For out val
		if(!validateWorldName(worldNameInput->label->GetText(), &validWorldName)) 
			return; // If the name is completely unusable, return
		
		// Set the world name on the db and load
		ChunkDatabase::Get()->SetWorldName(validWorldName);
		Engine::Get()->SetScene("game");
	});

	AddButton(engine, titleNew, "Back", 150.f, [] {
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
