export module Ultra.Media.Audio;

//#pragma warning(push, 0)
//import <AL/al.h>;
//import <AL/alext.h>;
////import <alc/alcmain.h>;
//import <AL/examples/common/alhelpers2.h>;
//import <AL/examples/common/alhelpers2.cpp>;
//
//import "vorbis/codec.h";
//import "vorbis/vorbisfile.h";
//#pragma warning(pop)

import Ultra.Core;
import Ultra.System.FileSystem;

namespace Ultra {

//static ALCdevice *s_AudioDevice = nullptr;
//
//static uint8_t *s_AudioScratchBuffer;
//static uint32_t s_AudioScratchBufferSize = 10 * 1024 * 1024; // 10mb initially

enum class SourceType {
    Unknown = 0,
    ogg
};

//static SourceType GetSourceType(const string &source) {
//    std::filesystem::path file = source;
//    string extension = file.extension().string();
//
//    if (extension == ".ogg") return SourceType::ogg;
//
//    return SourceType::Unknown;
//}

//static ALenum GetOpenALFormat(uint32_t channels) {
//    // Note: sample size is always 2 bytes (16-bits) with the .ogg decoder that we're using
//    switch (channels) {
//        case 1: return AL_FORMAT_MONO16;
//        case 2: return AL_FORMAT_STEREO16;
//    }
//    return 0;
//}

}

export namespace Ultra::Audio {

struct AudioDuration {
    uint32_t hours;
    uint32_t minutes;
    uint32_t seconds;
};

class Player;

class Source {
    friend Player;

public:
    Source() = default;
    Source(uint32_t handle, [[maybe_unused]] bool loaded, [[maybe_unused]] float length): BufferHandle(handle), Loaded(loaded) {}
    ~Source() {}

    static Source LoadFromFile(const string &file, bool spatial = false) {
        Source result = LoadSourceOgg(file);
        result.SetSpatial(spatial);
        return result;
    }

     // Accessors
    const AudioDuration GetDuration() const { return Duration; };
    const bool IsLoaded() const { return Loaded; };

    // Mutators
    void SetRepeat(bool value) {
        Repeat = value;
        //alSourcei(SourceHandle, AL_LOOPING, value ? AL_TRUE : AL_FALSE);
    }

    void SetGain(float value) {
        Gain = value;
        //alSourcef(SourceHandle, AL_GAIN, value);
    }
    void SetPitch(float value) {
        Pitch = value;
        //alSourcef(SourceHandle, AL_PITCH, value);
    }
    void SetPosition(float x, float y, float z) {
        Position[0] = x;
        Position[1] = y;
        Position[2] = z;

        //alSource3f(SourceHandle, AL_VELOCITY, 0, 0, 0);
        //alSourcefv(SourceHandle, AL_POSITION, Position);
    }
    void SetSpatial(bool value) {
        Spatial = value;

        //alSourcei(SourceHandle, AL_SOURCE_SPATIALIZE_SOFT, value ? AL_TRUE : AL_FALSE);
        //alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
    }


private:
    static Source LoadSourceOgg([[maybe_unused]] const string &file) {
        //FILE *f = fopen(file.c_str(), "rb");

        //OggVorbis_File vf;
        //if (ov_open_callbacks(f, &vf, NULL, 0, OV_CALLBACKS_NOCLOSE) < 0)
        //    std::cout << "Could not open ogg stream\n";

        //// Useful info
        //vorbis_info *vi = ov_info(&vf, -1);
        //auto sampleRate = vi->rate;
        //auto channels = vi->channels;
        //auto alFormat = GetOpenALFormat(channels);

        //uint64_t samples = ov_pcm_total(&vf, -1);
        //float trackLength = (float)samples / (float)sampleRate; // in seconds
        //uint32_t bufferSize = 2 * channels * samples; // 2 bytes per sample (I'm guessing...)

        //APP_LOG_DEBUG("[Ultra::Audio]: ", "'", file, "'");
        //APP_LOG_DEBUG(" - ", "Channels: ", channels);
        //APP_LOG_DEBUG(" - ", "Sample Rate: ", sampleRate);
        //APP_LOG_DEBUG(" - ", "Expected size: ", bufferSize);

        //// TODO: Replace with own buffer implementation
        //if (s_AudioScratchBufferSize < bufferSize) {
        //    s_AudioScratchBufferSize = bufferSize;
        //    delete[] s_AudioScratchBuffer;
        //    s_AudioScratchBuffer = new uint8_t[s_AudioScratchBufferSize];
        //}

        //uint8_t *oggBuffer = s_AudioScratchBuffer;
        //uint8_t *bufferPtr = oggBuffer;

        //int eof = 0;
        //while (!eof) {
        //    int current_section;
        //    long length = ov_read(&vf, (char *)bufferPtr, 4096, 0, 2, 1, &current_section);
        //    bufferPtr += length;
        //    if (length == 0) {
        //        eof = 1;
        //    } else if (length < 0) {
        //        if (length == OV_EBADLINK) {
        //            fprintf(stderr, "Corrupt bitstream section! Exiting.\n");
        //            exit(1);
        //        }
        //    }
        //}

        //uint32_t size = bufferPtr - oggBuffer;
        //// assert bufferSize == size
        //APP_LOG_DEBUG("  Read ", size, " bytes");

        //// Release file
        //ov_clear(&vf);
        //fclose(f);

        //ALuint buffer;
        //alGenBuffers(1, &buffer);
        //alBufferData(buffer, alFormat, oggBuffer, size, sampleRate);

        //Source result = { buffer, true, trackLength };
        //alGenSources(1, &result.SourceHandle);
        //alSourcei(result.SourceHandle, AL_BUFFER, buffer);

        //if (alGetError() != AL_NO_ERROR) APP_LOG_ERROR("Failed to setup sound source");

        //return result;
        return {};
    }

private:
    uint32_t BufferHandle = 0;
    uint32_t SourceHandle = 0;

    bool Loaded = false;
    bool Spatial = false;

    AudioDuration Duration = {};

    float Position[3] = {};
    float Gain = 1.0f;
    float Pitch = 1.0f;
    bool Repeat = false;
};


class Player {
    Player() = default;
    ~Player() = default;

public:
    Player(const Player &) = delete;
    Player(const Player &&) = delete;
    Player &operator=(const Player &) = delete;
    Player &operator=(const Player &&) = delete;

    static void Initialize();

    static Source LoadSource(const std::string &source);
    static void Play(const Source &source);
    static void Pause(const Source &source);
};


}
