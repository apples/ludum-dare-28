#ifndef AUDIODEVICE_HPP
#define AUDIODEVICE_HPP

#include <irrklang.h>

#include <map>
#include <string>

class AudioDevice
{
  public:
    using Engine = irrklang::ISoundEngine*;
    using Source = irrklang::ISoundSource*;
    using Sound  = irrklang::ISound*;

    static AudioDevice& inst();

    ~AudioDevice();

    Source loadSource(std::string filename);

    Sound play(Source src) const;
    Sound loop(Source src) const;

    void stop(Sound src) const;

  private:
    Engine engine;
    std::map<std::string, Source> sourceMap;

    AudioDevice();
};

#endif // AUDIODEVICE_HPP
