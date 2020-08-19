#include "Audio.h"

#pragma warning(push, 0)
#include <AL/al.h>
#include <AL/alext.h>
//#include <alc/alcmain.h>
#include <AL/examples/common/alhelpers2.h>
#include <AL/examples/common/alhelpers2.cpp>

#include "vorbis/codec.h"
#include "vorbis/vorbisfile.h"
#pragma warning(pop)

namespace Ultra::Audio {

static ALCdevice* s_AudioDevice = nullptr;

static uint8_t* s_AudioScratchBuffer;
static uint32_t s_AudioScratchBufferSize = 10 * 1024 * 1024; // 10mb initially



enum class SourceType {
    Unknown = 0,
    ogg
};

static SourceType GetSourceType(const string &source) {
    std::filesystem::path file = source;
    string extension = file.extension().string();

    if (extension == ".ogg") return SourceType::ogg;

    return SourceType::Unknown;
}



static ALenum GetOpenALFormat(uint32_t channels) {
    // Note: sample size is always 2 bytes (16-bits) with the .ogg decoder that we're using
    switch (channels) {
        case 1: return AL_FORMAT_MONO16;
        case 2: return AL_FORMAT_STEREO16;
    }
    return 0;
}



Source::Source(uint32_t handle, bool loaded, float length):
	BufferHandle(handle), Loaded(loaded) {
	// ToDo: Duration
}

Source::~Source() {
    // ToDo: Release sources
}


Source Source::LoadFromFile(const string &file, bool spatial) {
	Source result = LoadSourceOgg(file);
	result.SetSpatial(spatial);
	return result;
}

Source Source::LoadSourceOgg(const string &file) {
	FILE* f = fopen(file.c_str(), "rb");

	OggVorbis_File vf;
	if (ov_open_callbacks(f, &vf, NULL, 0, OV_CALLBACKS_NOCLOSE) < 0)
		std::cout << "Could not open ogg stream\n";

	// Useful info
	vorbis_info* vi = ov_info(&vf, -1);
	auto sampleRate = vi->rate;
	auto channels = vi->channels;
	auto alFormat = GetOpenALFormat(channels);

	uint64_t samples = ov_pcm_total(&vf, -1);
	float trackLength = (float)samples / (float)sampleRate; // in seconds
	uint32_t bufferSize = 2 * channels * samples; // 2 bytes per sample (I'm guessing...)

	APP_LOG_DEBUG("[Ultra::Audio]: ", "'", file, "'");
	APP_LOG_DEBUG(" - ", "Channels: ", channels);
	APP_LOG_DEBUG(" - ", "Sample Rate: ",  sampleRate);
	APP_LOG_DEBUG(" - ", "Expected size: ",  bufferSize);

	// TODO: Replace with own buffer implementation
	if (s_AudioScratchBufferSize < bufferSize) {
		s_AudioScratchBufferSize = bufferSize;
		delete[] s_AudioScratchBuffer;
		s_AudioScratchBuffer = new uint8_t[s_AudioScratchBufferSize];
	}

	uint8_t* oggBuffer = s_AudioScratchBuffer;
	uint8_t* bufferPtr = oggBuffer;

	int eof = 0;
	while (!eof) {
		int current_section;
		long length = ov_read(&vf, (char*)bufferPtr, 4096, 0, 2, 1, &current_section);
		bufferPtr += length;
		if (length == 0) {
			eof = 1;
		} else if (length < 0) {
			if (length == OV_EBADLINK) {
				fprintf(stderr, "Corrupt bitstream section! Exiting.\n");
				exit(1);
			}
		}
	}

	uint32_t size = bufferPtr - oggBuffer;
	// assert bufferSize == size
	APP_LOG_DEBUG("  Read ", size,  " bytes");

	// Release file
	ov_clear(&vf);
	fclose(f);

	ALuint buffer;
	alGenBuffers(1, &buffer);
	alBufferData(buffer, alFormat, oggBuffer, size, sampleRate);

	Source result = { buffer, true, trackLength };
	alGenSources(1, &result.SourceHandle);
	alSourcei(result.SourceHandle, AL_BUFFER, buffer);

	if (alGetError() != AL_NO_ERROR) APP_LOG_ERROR("Failed to setup sound source");

	return result;
}


// GetDuration return { (uint32_t)(m_TotalDuration / 60.0f), (uint32_t)m_TotalDuration % 60 };

void Source::SetRepeat(bool value) {
    Repeat = value;
	alSourcei(SourceHandle, AL_LOOPING, value ? AL_TRUE : AL_FALSE);
}

void Source::SetPosition(float x, float y, float z) {
    Position[0] = x;
    Position[1] = y;
    Position[2] = z;

    //alSource3f(SourceHandle, AL_VELOCITY, 0, 0, 0);
	alSourcefv(SourceHandle, AL_POSITION, Position);
}

void Source::SetGain(float value) {
    Gain = value;
    alSourcef(SourceHandle, AL_GAIN, value);
}

void Source::SetPitch(float value) {
    Pitch = value;
    alSourcef(SourceHandle, AL_PITCH, value);
}

void Source::SetSpatial(bool value) {
    Spatial = value;
    
	alSourcei(SourceHandle, AL_SOURCE_SPATIALIZE_SOFT, value ? AL_TRUE : AL_FALSE);
    alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
}



static void PrintAudioDeviceInfo()
{
	APP_LOG_DEBUG("Audio Device Info:");
	//APP_LOG_DEBUG("  Name: ", s_AudioDevice->DeviceName);
	//APP_LOG_DEBUG("  Sample Rate: ", s_AudioDevice->Frequency);
	//APP_LOG_DEBUG("  Max Sources: ", s_AudioDevice->SourcesMax);
	//APP_LOG_DEBUG("    Mono: ", s_AudioDevice->NumMonoSources);
	//APP_LOG_DEBUG("    Stereo: ", s_AudioDevice->NumStereoSources);
}

void Player::Initialize() {
	if (InitAL(s_AudioDevice, nullptr, 0) != 0) std::cout << "Audio device error!\n";

	PrintAudioDeviceInfo();

	s_AudioScratchBuffer = new uint8_t[s_AudioScratchBufferSize];

	// Init listener
	ALfloat listenerPos[] = { 0.0,0.0,0.0 };
	ALfloat listenerVel[] = { 0.0,0.0,0.0 };
	ALfloat listenerOri[] = { 0.0,0.0,-1.0, 0.0,1.0,0.0 };
	alListenerfv(AL_POSITION, listenerPos);
	alListenerfv(AL_VELOCITY, listenerVel);
	alListenerfv(AL_ORIENTATION, listenerOri);
}

Source Player::LoadSource(const std::string &source) {
	auto format = GetSourceType(source);
	switch (format) {
		case SourceType::ogg:  return Source::LoadFromFile(source);
	}

	// Loading failed or unsupported file type
	return { 0, false, 0 };
}

void Player::Play(const Source &source) {
    alSourcePlay(source.SourceHandle);
    // alGetSourcei(source.SourceHandle, AL_SOURCE_STATE, &s_PlayState);    // ALenum s_PlayState;
    // alGetSourcef(source.SourceHandle, AL_SEC_OFFSET, &offset);           // ALfloat offset;
}

void Player::Pause(const Source &source) {
    alSourcePause(source.SourceHandle);
}

}
