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
	ma_result result;
	//ma_sound* newSound = new ma_sound();

	//result = ma_sound_init_from_file(&_Instance->engine, path.c_str(),
	//	MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_DECODE |
	//	MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_ASYNC,
	//	NULL, NULL, newSound);

	//if(result == MA_SUCCESS) {
	//	_Instance->_audioClips[name] = newSound;
	//}
	//else {
	//	assert(false);
	//}
	//_Instance->_audioClips[name] = path;

	//ma_engine_play_sound(&_Instance->engine, path.c_str(), NULL);

	_Instance->soundPaths[name] = path;

}

void Audio::LoadClipStream(const string& path, const string& name, bool is3d) {
	ma_result result;
	/*ma_sound* newSound = new ma_sound();

	result = ma_sound_init_from_file(&_Instance->engine, path.c_str(), 
		MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_DECODE |
		MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_ASYNC |
		MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_STREAM,
		NULL, NULL, newSound);

	if(result == MA_SUCCESS) {
		_Instance->_audioClips[name] = newSound;
	}
	else {
		assert(false);
	}*/
}

void Audio::Play(const string& name, Vector3 worldPos, float volume) {
}

void Audio::Play(const string& name, float volume) {
	auto findPath = _Instance->soundPaths.find(name);
	if(findPath != _Instance->soundPaths.end()) {
		const string& path = findPath->second;

		Sound sound;

		auto findDecoder = _Instance->decoders.find(path);
		if(findDecoder == _Instance->decoders.end()) {
			// sound not found in decoders, make a new decoder for this sound
			ma_decoder* newDecoder = new ma_decoder();
			ma_decoder_init_file(path.c_str(), NULL, newDecoder);

			_Instance->decoders[path] = newDecoder;
			sound.decoder = newDecoder;
		}
		else {
			sound.decoder = findDecoder->second;
		}

		//sound.currFrame = 0;
		//sound.isAtEnd = MA_FALSE;

		_Instance->sounds.push_back(sound);
	}


	//auto findIt = _Instance->_audioClips.find(name);
	//if(findIt != _Instance->_audioClips.end()) {
	//	ma_result res;
	//	
	//	//_Instance->engines[0]
	//	res = ma_sound_start(findIt->second);
	//	//res = ma_engine_play_sound(&_Instance->engine, findIt->second.c_str(), NULL);
	//	if(res != MA_SUCCESS) {
	//		assert(false);
	//	}
	//}
}

void Audio::Update() {

}

void Audio::SetListener(const Transform* t) {
}

ma_uint32 read_and_mix_pcm_frames_f32(ma_decoder* pDecoder, float* pOutputF32, ma_uint32 frameCount)
{
	/*
	The way mixing works is that we just read into a temporary buffer, then take the contents of that buffer and mix it with the
	contents of the output buffer by simply adding the samples together. You could also clip the samples to -1..+1, but I'm not
	doing that in this example.
	*/
	ma_result result;
	float temp[4096];
	ma_uint32 tempCapInFrames = ma_countof(temp) / CHANNEL_COUNT;
	ma_uint32 totalFramesRead = 0;

	while(totalFramesRead < frameCount) {
		ma_uint64 iSample;
		ma_uint64 framesReadThisIteration;
		ma_uint32 totalFramesRemaining = frameCount - totalFramesRead;
		ma_uint32 framesToReadThisIteration = tempCapInFrames;
		if(framesToReadThisIteration > totalFramesRemaining) {
			framesToReadThisIteration = totalFramesRemaining;
		}

		//result = ma_decoder_read_pcm_frames(pDecoder, temp, framesToReadThisIteration, &framesReadThisIteration);
		result = ma_decoder_read_pcm_frames(pDecoder, pOutputF32, framesToReadThisIteration, &framesReadThisIteration);
		if(result != MA_SUCCESS || framesReadThisIteration == 0) {
			break;
		}

		/* Mix the frames together. */
		for(iSample = 0; iSample < framesReadThisIteration * CHANNEL_COUNT; ++iSample) {
			pOutputF32[totalFramesRead * CHANNEL_COUNT + iSample] += temp[iSample];
		}

		totalFramesRead += (ma_uint32)framesReadThisIteration;

		if(framesReadThisIteration < (ma_uint32)framesToReadThisIteration) {
			break;  /* Reached EOF. */
		}
	}

	return totalFramesRead;
}

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
	float* pOutputF32 = (float*)pOutput;
	//float temp[4096];
	//ma_engine_read_pcm_frames((ma_engine*)pDevice->pUserData, pOutput, frameCount, NULL);

	//for(auto& sound : Audio::_Instance->sounds) {
	for(auto it = Audio::_Instance->sounds.begin(); it != Audio::_Instance->sounds.end();) {
		auto& sound = *it;
		//ma_uint64 length;
		//ma_decoder_get_length_in_pcm_frames(sound.decoder, &length);
		//if(sound.isPlaying && sound.currFrame < length) {
		//	//ma_decoder_read_pcm_frames(sound.decoder, )
		//}
		
		ma_uint32 framesRead = read_and_mix_pcm_frames_f32(sound.decoder, pOutputF32, frameCount);

		//ma_uint64 framesRead;
		////ma_uint32 
		//unsigned int i = 0;
		//while(i < frameCount) {

		//	ma_decoder_read_pcm_frames(sound.decoder, pOutputF32, frameCount, &framesRead);
		//}

		if(framesRead < frameCount) {
			//sound.isAtEnd = true;
			//sound.decoder->readPointerInPCMFrames
			ma_decoder_seek_to_pcm_frame(sound.decoder, 0);
			it = Audio::_Instance->sounds.erase(it);
		}
		else {
			++it;
		}

	}

	(void)pInput;
}

#define DECODER_COUNT 5

void Audio::Init() {
	_Instance = this;
	ma_result result;

	ma_device_config deviceCfg = ma_device_config_init(ma_device_type_playback);
	deviceCfg.playback.format = SAMPLE_FORMAT;
	deviceCfg.playback.channels = CHANNEL_COUNT;
	deviceCfg.sampleRate = SAMPLE_RATE;
	deviceCfg.dataCallback = data_callback;

	result = ma_device_init(NULL, &deviceCfg, &device);
	ErrHandle(result, "Failed to initialise device");

	result = ma_device_start(&device);
	ErrHandle(result, "Failed to start device");












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

	sounds.clear();
}

