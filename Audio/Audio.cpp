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
			
			//return; //todo: vorbis
		}
		
		res = ma_sound_init_from_file(&_Instance->engine, findIt->second.path.c_str(), flags, NULL, NULL, newSound);
		ErrHandle(res, "Unable to initialise sound from file");
		
		//newSound->endCallback = OnSoundEnd; //this doesnt work, i think it deleted the sound data in memory

		res = ma_sound_start(newSound);
		//res = ma_engine_play_sound(&_Instance->engine, findIt->second.c_str(), NULL);
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

	// device / decoder config
	//ma_device_config deviceCfg = ma_device_config_init(ma_device_type_playback);
	//deviceCfg.playback.format = SAMPLE_FORMAT;
	//deviceCfg.playback.channels = CHANNEL_COUNT;
	//deviceCfg.sampleRate = SAMPLE_RATE;
	//deviceCfg.dataCallback = data_callback;

	//result = ma_device_init(NULL, &deviceCfg, &device);
	//ErrHandle(result, "Failed to initialise device");

	//result = ma_device_start(&device);
	//ErrHandle(result, "Failed to start device");


	result = ma_engine_init(NULL, &engine);
	ErrHandle(result, "Failed to initialise engine");









	//ma_decoder decoder;
	//ma_device_config deviceConfig;
	//ma_device device;

	//result = ma_decoder_init(file)
	
	/*result = ma_engine_init(NULL, &engine);
	ErrHandle(result, "Failed to initialise engine");

	ma_decoder_config decoderConfig = ma_decoder_config_init(SAMPLE_FORMAT, CHANNEL_COUNT, SAMPLE_RATE);
	for(int i = 0; i < DECODER_COUNT; i++) {
		result = ma_decoder_init
	}*/

	
	//ma_result result;
	//ma_uint32 playbackDeviceCount;

	//result = ma_context_init(NULL, 0, NULL, &ctx);

	////// Get devices
	//ma_device_info* pDeviceInfos;
	//result = ma_context_get_devices(&ctx, &pDeviceInfos, &playbackDeviceCount, NULL, NULL);
	//// Get the default devices index
	//int chosenDeviceIndex = 0;
	//for (int di = 0; di< playbackDeviceCount; di++) {
	//	if (&pDeviceInfos[di].isDefault) chosenDeviceIndex = di;
	//}
	////
	//ma_resource_manager_config resMgrConfig = ma_resource_manager_config_init();
	//resMgrConfig.decodedFormat = ma_format_f32;
	//resMgrConfig.decodedChannels = 0;
	//resMgrConfig.decodedSampleRate = 44100;
	//result = ma_resource_manager_init(&resMgrConfig, &resMgr);
	//ErrHandle(result, "Failed to init resource manager");

	//ma_device_config deviceConfig = ma_device_config_init(ma_device_type_playback);
	//deviceConfig.playback.pDeviceID = &pDeviceInfos[chosenDeviceIndex].id;
	//deviceConfig.playback.format = resMgr.config.decodedFormat;
	//deviceConfig.playback.channels = 0;
	//deviceConfig.sampleRate = resMgr.config.decodedSampleRate;
	//deviceConfig.dataCallback = data_callback;
	//deviceConfig.pUserData = &engine;
	//result = ma_device_init(&ctx, &deviceConfig, &device);
	//ErrHandle(result, "Failed to initialize audio device");

	//ma_engine_config engineConfig = ma_engine_config_init();
	//engineConfig.pDevice = &device;
	//engineConfig.pResourceManager = &resMgr;
	//engineConfig.noAutoStart = MA_TRUE;
	//result = ma_engine_init(&engineConfig, &engine);
	//ErrHandle(result, "Failed to initialize engine ");

	//result = ma_engine_start(&engine);
	//ErrHandle(result, "Failed to start engine ");








	//for (int i = 0; i < MAX_DEVICES; i++) {
	//	ma_device_config deviceConfig = ma_device_config_init(ma_device_type_playback);
	//	deviceConfig.playback.pDeviceID = &pDeviceInfo[chosenDeviceIndex].id;
	//	deviceConfig.playback.format = resMgr.config.decodedFormat;
	//	deviceConfig.playback.channels = 0;
	//	deviceConfig.sampleRate = resMgr.config.decodedSampleRate;
	//	deviceConfig.dataCallback = data_callback;
	//	deviceConfig.pUserData = &engines[i];


	//	ma_engine_config engineConfig = ma_engine_config_init();
	//	engineConfig.pDevice = &devices[i];
	//	engineConfig.pResourceManager = &resMgr;
	//	engineConfig.noAutoStart = MA_TRUE;

	//	ma_engine* currentEngine = &engines[i];

	//	result = ma_engine_init(&engineConfig, currentEngine);

	//	if(ErrHandle(result, "Failed to initialize engine " + i)) continue;

	//	result = ma_engine_start(currentEngine);

	//	if (ErrHandle(result, "Failed to start engine " + i)) continue;

	//	//ma_sound_init_from_file()

	//}

	//result = ma_engine_init(NULL, &engine);
}

Audio::Audio() {
	Init();
}

Audio::~Audio() {
	if(this == _Instance) {
		_Instance = nullptr;
	}
	//ma_engine_uninit(&engines[0]);

	//for(const auto& kvp : _audioClips) {
	//	delete kvp.second;
	//}
	//_audioClips.clear();

	ma_device_uninit(&device);

	for(const auto& kvp : decoders) {
		ma_decoder_uninit(kvp.second);
		delete kvp.second;
	}
	decoders.clear();

	_sounds.clear();
}

