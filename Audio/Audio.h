#pragma once

#include <string>
#include <map>
#include <assert.h>
#include <fmod.hpp>
#include <fmod_studio.hpp>
#include "../Engine/MathUtil.h"

using namespace std;

class Transform;

#define AUDIO_SAMPLE_RATE 44100
#define MAX_AUDIO_CHANNELS 100
#define DISTANCEFACTOR 1.f

void ERRCHECK(FMOD_RESULT result);

class Audio {
private:
	const Transform* pListener = nullptr;

	FMOD::Channel* fChannel = nullptr;
	
	//FMOD::Sound* fSound;
	map<string, FMOD::Sound*> _audioClips = {};


	FMOD::ChannelGroup* fChannelGroup = nullptr;
	FMOD::System* fSystem = nullptr;
	FMOD::Studio::System* fStudioSys = nullptr;

	static Audio* _Instance;
public:

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