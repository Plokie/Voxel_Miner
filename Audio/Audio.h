#pragma once

#include <string>
#include <map>
#include <list>
#include <vector>
#include <assert.h>
#include "../Engine/MathUtil.h"
#include "miniaudio.h"

using namespace std;

class Transform;

#define MAX_DEVICES 2
#define MAX_VOICES 32

struct SoundStore {
	string path = "";
	bool isStream = false;

	SoundStore(){}
	SoundStore(const string& path, bool isStream = false) : path(path), isStream(isStream){}
};

class Audio {
private:
	const Transform* _pListener;

	map<string, ma_decoder*> decoders;

	ma_engine engine;
	ma_engine engine3d; //maybe dont need 2 engines? theres like an id thing i saw

	ma_device device;

	map<string, SoundStore> _sounds = {};
public:
	static Audio* _Instance;

	static void LoadClip(const string& path, const string& name, bool is3d = false);
	static void LoadClipStream(const string& path, const string& name, bool is3d = false);

	static void Play(const string& name, Vector3 worldPos, float volume);
	static void Play(const string& name, float volume);

	static void SetListener(const Transform* t);

	void Update();
	void Init();
	Audio();
	~Audio();
};