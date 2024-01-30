#include "Audio.h"

bool ErrHandle(ma_result result, string message) {
	if (result != MA_SUCCESS) {

		// todo: print message here
		assert(false);
		return true;
	}

	return false;
}

void Audio::LoadClip(const string& path, const string& name, bool is3d) {
	ma_result result;
	ma_sound newSound;

	result = ma_sound_init_from_file(&_Instance->engines[0], path.c_str(), 0, NULL, NULL, &newSound);
	if(result == MA_SUCCESS) {

	}
	else {
		assert(false);
	}


}

void Audio::LoadClipStream(const string& path, const string& name, bool is3d) {
}

void Audio::Play(const string& name, Vector3 worldPos, float volume) {
}

void Audio::Play(const string& name, float volume) {

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

void Audio::Init() {
	ma_result result;
	ma_uint32 playbackDeviceCount;

	ma_context ctx;
	result = ma_context_init(NULL, 0, NULL, &ctx);

	// Get devices
	ma_device_info* pDeviceInfo;
	result = ma_context_get_devices(&ctx, &pDeviceInfo, &playbackDeviceCount, NULL, NULL);
	// Get the default devices index
	int chosenDeviceIndex = 0;
	for (int di = 0; di< playbackDeviceCount; di++) {
		if (&pDeviceInfo[di].isDefault) chosenDeviceIndex = di;
	}
	
	ma_resource_manager_config resMgrConfig = ma_resource_manager_config_init();
	resMgrConfig.decodedFormat = ma_format_f32;
	resMgrConfig.decodedChannels = 0;
	resMgrConfig.decodedSampleRate = 44100;
	result = ma_resource_manager_init(&resMgrConfig, &resMgr);
	if (ErrHandle(result, "Failed to init resource manager")) exit(777);

	for (int i = 0; i < MAX_DEVICES; i++) {
		ma_device_config deviceConfig = ma_device_config_init(ma_device_type_playback);
		deviceConfig.playback.pDeviceID = &pDeviceInfo[chosenDeviceIndex].id;
		deviceConfig.playback.format = resMgr.config.decodedFormat;
		deviceConfig.playback.channels = 0;
		deviceConfig.sampleRate = resMgr.config.decodedSampleRate;
		deviceConfig.dataCallback = data_callback;
		deviceConfig.pUserData = &engines[i];


		ma_engine_config engineConfig = ma_engine_config_init();
		engineConfig.pDevice = &devices[i];
		engineConfig.pResourceManager = &resMgr;
		engineConfig.noAutoStart = MA_TRUE;

		ma_engine* currentEngine = &engines[i];

		result = ma_engine_init(&engineConfig, currentEngine);

		if(ErrHandle(result, "Failed to initialize engine " + i)) continue;

		result = ma_engine_start(currentEngine);

		if (ErrHandle(result, "Failed to start engine " + i)) continue;

		//ma_sound_init_from_file()

	}

	//result = ma_engine_init(NULL, &engine);
}

Audio::Audio() {
	Init();
}

Audio::~Audio() {

}

