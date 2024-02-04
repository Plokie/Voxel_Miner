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

/// <summary>
/// 
/// </summary>
struct SoundStore {
	//ma_decoder* decoder;

	//ma_sound sound;
	string path = "";
	bool isStream = false;

	SoundStore(){}
	SoundStore(const string& path, bool isStream = false) : path(path), isStream(isStream){}
};

class Audio {
private:
	//map<string, string> soundPaths;
	map<string, ma_decoder*> decoders;
	//ma_resource_manager resMgr;
	////ma_engine engines[MAX_DEVICES];
	ma_engine engine;
	//ma_context ctx;
	////ma_device devices[MAX_DEVICES];
	ma_device device;


	//map<string, ma_sound*> _audioClips = {};

	map<string, SoundStore> _sounds = {};
public:
	//list<Sound> sounds;
	//ma_event g_stopEvent;
	//vector<pair<ma_decoder,bool>> decoders;

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