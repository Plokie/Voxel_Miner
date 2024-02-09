#include "Audio.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include "../Engine/Transform.h"

Audio* Audio::_Instance = nullptr;

bool ErrHandle(ma_result result, string message) {
	if (result != MA_SUCCESS) {

		// todo: print message here
		assert(false);
		return true;
	}

	return false;
}

void Audio::LoadClip(const string& path, const string& name, bool is3d) {
	_Instance->_sounds[name] = { path, false };

}

void Audio::LoadClipStream(const string& path, const string& name, bool is3d) {
	_Instance->_sounds[name] = { path, true };
}

void Audio::Play(const string& name, Vector3 worldPos, float volume) {
	auto findIt = _Instance->_sounds.find(name);
	if (findIt != _Instance->_sounds.end()) {
		ma_result res;

		ma_sound* newSound = new ma_sound();

		ma_uint32 flags = MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_DECODE | MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_ASYNC;
		if (findIt->second.isStream) {
			flags = MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_DECODE | MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_ASYNC | MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_STREAM;
		}

		ma_sound_set_position(newSound, worldPos.x, worldPos.y, worldPos.z);
		//todo: spatial sound settings (conical, vol, attentuation etc)

		res = ma_sound_init_from_file(&_Instance->engine3d, findIt->second.path.c_str(), flags, NULL, NULL, newSound);
		//ma_sound_init_copy()
		ErrHandle(res, "Unable to initialise sound from file");

		//newSound->endCallback = OnSoundEnd; //this doesnt work

		res = ma_sound_start(newSound);
		ErrHandle(res, "Unable to play sound");
	}
}

void OnSoundEnd(void* pUserData, ma_sound* pSound) {
	//this doesnt work, it completely deletes the sound, cant be replayed again
	ma_sound_uninit(pSound);
	delete pSound;
}

void Audio::Play(const string& name, float volume) {
	auto findIt = _Instance->_sounds.find(name);
	if(findIt != _Instance->_sounds.end()) {
		ma_result res;
		
		ma_sound* newSound = new ma_sound(); //hmm memory leak

		ma_uint32 flags = MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_DECODE | MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_ASYNC;
		if(findIt->second.isStream) {
			flags = MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_DECODE | MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_ASYNC | MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_STREAM;
		}
		
		res = ma_sound_init_from_file(&_Instance->engine, findIt->second.path.c_str(), flags, NULL, NULL, newSound);
		ErrHandle(res, "Unable to initialise sound from file");
		
		//newSound->endCallback = OnSoundEnd; //this doesnt work

		res = ma_sound_start(newSound);
		ErrHandle(res, "Unable to play sound");
	}
}

void Audio::Update() {
	ma_engine_listener_set_position(&engine3d, 0, _pListener->position.x, _pListener->position.y, _pListener->position.z);
	Vector3 fwd = _pListener->forward();
	ma_engine_listener_set_direction(&engine3d, 0, fwd.x, fwd.y, fwd.z);
}

void Audio::SetListener(const Transform* t) {
	_Instance->_pListener = t;
}

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
	(void)pInput;
	ma_engine_read_pcm_frames((ma_engine*)pDevice->pUserData, pOutput, frameCount, NULL);
}

void Audio::Init() {
	_Instance = this;
	ma_result result;

	result = ma_engine_init(NULL, &engine);
	ErrHandle(result, "Failed to initialise 2d sound engine");

	result = ma_engine_init(NULL, &engine3d);
	ErrHandle(result, "Failed to initialise 3d sound engine");
}

Audio::Audio() {
	Init();
}

Audio::~Audio() {
	if(this == _Instance) _Instance = nullptr;

	ma_device_uninit(&device);
	ma_engine_uninit(&engine);
	ma_engine_uninit(&engine3d);

	for(const auto& kvp : decoders) {
		ma_decoder_uninit(kvp.second);
		delete kvp.second;
	}
	decoders.clear();

	_sounds.clear();
}

