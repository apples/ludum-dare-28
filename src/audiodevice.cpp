#include "audiodevice.hpp"

#include <exception>

using namespace irrklang;
using namespace std;

class AudioDeviceE_BadInit
    : public exception
{
public:
    virtual const char* what() const noexcept override
    {
        return "Could not initialize audio device!";
    }
};

AudioDevice& AudioDevice::inst() //static
{
    static AudioDevice instance;
    return instance;
}

AudioDevice::AudioDevice()
    : engine(createIrrKlangDevice())
{
    if (!engine) throw AudioDeviceE_BadInit();
}

AudioDevice::~AudioDevice()
{
    engine->drop();
}

AudioDevice::Source AudioDevice::loadSource(string filename)
{
    auto iter = sourceMap.find(filename);
    if (iter != sourceMap.end()) return iter->second;
    auto ptr = engine->addSoundSourceFromFile(filename.c_str(), ESM_NO_STREAMING, true);
    iter = sourceMap.insert(iter, {std::move(filename), ptr});
    return iter->second;
}

AudioDevice::Sound AudioDevice::play(Source src) const
{
    return engine->play2D(src, false, false, true, true);
}

AudioDevice::Sound AudioDevice::loop(Source src) const
{
    return engine->play2D(src, true, false, true, true);
}

void AudioDevice::stop(Sound src) const
{
    src->stop();
    src->drop();
}

void AudioDevice::quickPlay(const std::string& filename) const
{
    engine->play2D(filename.c_str());
}
