#include "Audio.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#define SAMPLE_FORMAT   ma_format_f32
#define CHANNEL_COUNT   2
#define SAMPLE_RATE     44100

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
	_Instance->_sounds[name] = { path };

}

void Audio::LoadClipStream(const string& path, const string& name, bool is3d) {
	_Instance->_sounds[name] = { path, true };
}

void Audio::Play(const string& name, Vector3 worldPos, float volume) {
}

void OnSoundEnd(void* pUserData, ma_sound* pSound) {
	//this doesnt work, i think it deletes the sound data in memory?
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
		
		//newSound->endCallback = OnSoundEnd; //this doesnt work, i think it deleted the sound data in memory

		res = ma_sound_start(newSound);

		if(res != MA_SUCCESS) {
			assert(false);
		}
	}
}

void Audio::Update() {

}

void Audio::SetListener(const Transform* t) {
}

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
	(void)pInput;
	ma_engine_read_pcm_frames((ma_engine*)pDevice->pUserData, pOutput, frameCount, NULL);
}

#define DECODER_COUNT 5

void Audio::Init() {
	_Instance = this;
	ma_result result;

	result = ma_engine_init(NULL, &engine);
	ErrHandle(result, "Failed to initialise engine");
}

Audio::Audio() {
	Init();
}

Audio::~Audio() {
	if(this == _Instance) _Instance = nullptr;

	ma_device_uninit(&device);
	ma_engine_uninit(&engine);

	for(const auto& kvp : decoders) {
		ma_decoder_uninit(kvp.second);
		delete kvp.second;
	}
	decoders.clear();

	_sounds.clear();
}

