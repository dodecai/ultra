#pragma once

#include <Ultra.pch>
#include <Core.h>

namespace Ultra::Audio {

struct AudioDuration {
    uint32_t hours;
    uint32_t minutes;
    uint32_t seconds;
};


class Player;

class Source {
    friend Player;

    uint32_t BufferHandle = 0;
    uint32_t SourceHandle = 0;

    bool Loaded = false;
    bool Spatial = false;

    AudioDuration Duration = {};

    float Position[3] = {};
    float Gain = 1.0f;
    float Pitch = 1.0f;
    bool Repeat = false;

public:
    Source() = default;
    ~Source();

   static Source LoadFromFile(const string &file, bool spatial = false);

    // Accessors
    const AudioDuration GetDuration() const { return Duration; };
    const bool IsLoaded() const { return Loaded; };

    // Mutators
    void SetRepeat(bool value);

    void SetGain(float value);
    void SetPitch(float value);
    void SetPosition(float x, float y, float z);
    void SetSpatial(bool value);


private:
    Source(uint32_t handle, bool loaded, float length);

    static Source LoadSourceOgg(const string &file);
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
