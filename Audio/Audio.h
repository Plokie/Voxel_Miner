#pragma once

#include <string>
#include <map>
#include <assert.h>
#include "../Engine/MathUtil.h"
#include "miniaudio.h"

using namespace std;

class Transform;

#define MAX_DEVICES 2
#define MAX_VOICES 32

class Audio {
private:
	ma_resource_manager resMgr;
	ma_engine engines[MAX_DEVICES];
	ma_device devices[MAX_DEVICES];

	map<string, ma_sound> _audioClips = {};
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