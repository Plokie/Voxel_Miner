#include "AudioFMOD.h"

#include "../Engine/Transform.h"
//#include <fmod_errors.h>

Audio* Audio::_Instance = nullptr;

void Audio::LoadClip(const string& path, const string& name, bool is3d)
{
	//todo: make parameters
	bool loop = false;

	assert(_Instance);

	FMOD::Sound* newSound;
	ERRCHECK(_Instance->fSystem->createSound(path.c_str(), is3d ? FMOD_3D : FMOD_2D, 0, &newSound));
	ERRCHECK(newSound->setMode(loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF));
	ERRCHECK(newSound->set3DMinMaxDistance(0.5f * DISTANCEFACTOR, 1000.0f * DISTANCEFACTOR));
	_Instance->_audioClips[name] = newSound;	
}

void Audio::LoadClipStream(const string& path, const string& name, bool is3d) {
	assert(_Instance);
	bool loop = false;

	FMOD::Sound* newSound;
	ERRCHECK(_Instance->fSystem->createStream(path.c_str(), is3d?FMOD_3D:FMOD_2D, 0, &newSound));
	ERRCHECK(newSound->setMode(loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF));
	ERRCHECK(newSound->set3DMinMaxDistance(0.5f * DISTANCEFACTOR, 1000.0f * DISTANCEFACTOR));
	_Instance->_audioClips[name] = newSound;
}

void Audio::Play(const string& name, Vector3 worldPos, float volume)
{
	assert(_Instance);
	assert(_Instance->_audioClips[name]);

	if(_Instance) {
		FMOD::Channel* channel;
		ERRCHECK(_Instance->fSystem->playSound(_Instance->_audioClips[name], _Instance->fChannelGroup, true, &channel));
		channel->setVolume(volume);

		FMOD_VECTOR position = { worldPos.x,worldPos.y,worldPos.z };
		FMOD_VECTOR velocity = { 0.f,0.f,0.f };

		ERRCHECK(channel->set3DAttributes(&position, &velocity));


		ERRCHECK(channel->setPaused(false));
	}
}

void Audio::Play(const string& name, float volume)
{
	assert(_Instance);
	assert(_Instance->_audioClips[name]);

	if(_Instance) {
		FMOD::Channel* channel;
		ERRCHECK(_Instance->fSystem->playSound(_Instance->_audioClips[name], _Instance->fChannelGroup, true, &channel));
		channel->setVolume(volume);
		ERRCHECK(channel->setPaused(false));
	}
}

void Audio::Update()
{
	if(pListener != nullptr) {
		FMOD_VECTOR position = { pListener->position.x, pListener->position.y, pListener->position.z };

		Vector3 v3fwd = pListener->forward();
		FMOD_VECTOR forward = { v3fwd.x, v3fwd.y, v3fwd.z };

		Vector3 v3up = pListener->up();
		FMOD_VECTOR up = { v3up.x, v3up.y, v3up.z };

		ERRCHECK(_Instance->fSystem->set3DListenerAttributes(0, &position, 0, &forward, &up));
	}

	fStudioSys->update();
}

void Audio::SetListener(const Transform* t) {
	if(_Instance) {
		_Instance->pListener = t;
	}
}

void Audio::Init()
{
	if(_Instance == nullptr) {
		_Instance = this;
	}

	ERRCHECK(FMOD::Studio::System::create(&fStudioSys));
	ERRCHECK(fStudioSys->getCoreSystem(&fSystem));
	ERRCHECK(fSystem->setSoftwareFormat(AUDIO_SAMPLE_RATE, FMOD_SPEAKERMODE_STEREO, 0));
	ERRCHECK(fSystem->set3DSettings(1.0, DISTANCEFACTOR, 0.5f));
	ERRCHECK(fStudioSys->initialize(MAX_AUDIO_CHANNELS, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0));
	ERRCHECK(fSystem->getMasterChannelGroup(&fChannelGroup));

	FMOD_VECTOR position = { 0.f,0.f,0.f };
	FMOD_VECTOR forward = { 0.f,0.f,1.f };
	FMOD_VECTOR up = { 0.f, 1.f, 0.f };
	ERRCHECK(_Instance->fSystem->set3DListenerAttributes(0, &position, 0, &forward, &up));
}

Audio::Audio() {
	Init();
}

Audio::~Audio()
{
	fSystem->close();
	fStudioSys->release();
}

void ERRCHECK(FMOD_RESULT result)
{
	if(result != FMOD_OK) assert(false);
}
