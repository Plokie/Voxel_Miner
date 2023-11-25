#include "Audio.h"


//#include <fmod_errors.h>

void Audio::LoadClip(const string& path, const string& name)
{
	//FMOD::Sound* newSound;
	//ERRCHECK(fSystem->createSound(path.c_str(), 0, 0, &newSound));
}

void Audio::Init()
{
	//FMOD_RESULT      result;
	//unsigned int     version;
	//FMOD_SPEAKERMODE speakermode;
	//FMOD_CAPS        caps;

	//ERRCHECK(FMOD::System_Create(&fSystem));
	//ERRCHECK(fSystem->getVersion(&version));
	//if(version < FMOD_VERSION) {
	//	assert(false);
	//}

	//ERRCHECK(fSystem->getDriverCaps(0, &caps, 0, 0, &speakermode));
	//ERRCHECK(fSystem->setSpeakerMode(speakermode));

	//if(caps & FMOD_CAPS_HARDWARE_EMULATED) {
	//	ERRCHECK(fSystem->setDSPBufferSize(1024, 10));
	//}

	//result = fSystem->init(100, FMOD_INIT_NORMAL, 0);
	//if(result == FMOD_ERR_OUTPUT_CREATEBUFFER) {
	//	ERRCHECK(fSystem->setSpeakerMode(FMOD_SPEAKERMODE_STEREO));
	//	ERRCHECK(fSystem->init(100, FMOD_INIT_NORMAL, 0));
	//}

	ERRCHECK(FMOD::Studio::System::create(&fStudioSys));
	ERRCHECK(fStudioSys->getCoreSystem(&fSystem));
	ERRCHECK(fSystem->setSoftwareFormat(AUDIO_SAMPLE_RATE, FMOD_SPEAKERMODE_STEREO, 0));
	ERRCHECK(fSystem->set3DSettings(1.0, DISTANCEFACTOR, 0.5f));
	ERRCHECK(fStudioSys->initialize(MAX_AUDIO_CHANNELS, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0));
	ERRCHECK(fSystem->getMasterChannelGroup(&fChannelGroup));
}

Audio::Audio() {
	Init();
}

Audio::~Audio()
{
	fSystem->close();
}

void ERRCHECK(FMOD_RESULT result)
{
	if(result != FMOD_OK) assert(false);
}
