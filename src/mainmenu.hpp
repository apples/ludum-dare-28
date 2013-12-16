#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include "audiodevice.hpp"
#include "screen.hpp"

#include "inugami/core.hpp"
#include "inugami/texture.hpp"

class MainMenu
    : public Screen
{
    Inugami::Core& core;

    Inugami::Texture background;

    AudioDevice::Source bgm;
    AudioDevice::Sound bgmtrack;

public:
    MainMenu(Inugami::Core& c);

    virtual bool isOpaque() const override;
    virtual bool isTunnel() const override;

    virtual Event tick() override;
    virtual void draw() override;
};

#endif // MAINMENU_HPP
