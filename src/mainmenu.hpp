#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include "screen.hpp"

#include "inugami/core.hpp"

class MainMenu
    : public Screen
{
    Inugami::Core& core;

public:
    MainMenu(Inugami::Core& c);

    virtual bool isOpaque() const override;
    virtual bool isTunnel() const override;

    virtual Event tick() override;
    virtual void draw() override;
};

#endif // MAINMENU_HPP
