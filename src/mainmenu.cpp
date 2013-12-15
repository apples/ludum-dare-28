#include "mainmenu.hpp"

#include "audiodevice.hpp"

#include "inugami/interface.hpp"

MainMenu::MainMenu(Inugami::Core& c)
    : core(c)
{}

bool MainMenu::isOpaque() const
{
    return true;
}

bool MainMenu::isTunnel() const
{
    return false;
}

Screen::Event MainMenu::tick()
{
    auto keyESC = core.iface->key(0_ivkFunc);

    if (keyESC) return {Event::POP, this};

    return {Event::NONE, nullptr};
}

void MainMenu::draw()
{

}
