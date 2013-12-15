#ifndef AUDIODEVICE_HPP
#define AUDIODEVICE_HPP

#include <irrklang.h>

#include <map>
#include <string>

class AudioDevice
{
    using Engine = irrklang::ISoundEngine*;
    using Source = irrklang::ISoundSource*;
    using Sound  = irrklang::ISound*;

    Engine engine;
    std::map<std::string, Source> sourceMap;

    AudioDevice();

public:
    static AudioDevice& inst();

    ~AudioDevice();

    Source loadSource(std::string filename);

    Sound play(Source src) const;
    Sound loop(Source src) const;

    void stop(Sound src) const;
};

#endif // AUDIODEVICE_HPP
