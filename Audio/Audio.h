#pragma once

#include <string>
#include <map>
#include <assert.h>
#include <fmod.hpp>
#include <fmod_studio.hpp>

using namespace std;

//fwd dec
//namespace FMOD {
//	class System;
//	class Sound;
//	class Channel;
//	class ChannelGroup;
//}

//enum class FMOD_RESULT : unsigned int;


/*
FMOD::Sound is a single AudioClip

FMOD::Channel is the output channel of audio clips. Think of it as the AudioSource

FMOD::ChannelGroup is 
*/

#define AUDIO_SAMPLE_RATE 44100
#define MAX_AUDIO_CHANNELS 100
#define DISTANCEFACTOR 1.f

void ERRCHECK(FMOD_RESULT result);

class Audio {
private:

	FMOD::Channel* fChannel;
	
	//FMOD::Sound* fSound;
	map<string, FMOD::Sound*> _audioClips = {};


	FMOD::ChannelGroup* fChannelGroup;
	FMOD::System* fSystem;
	FMOD::Studio::System* fStudioSys;
public:

	void LoadClip(const string& path, const string& name);





	void Init();
	Audio();
	~Audio();
};