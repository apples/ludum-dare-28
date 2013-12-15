#include "mainmenu.hpp"

#include "audiodevice.hpp"
#include "meta.hpp"

#include "inugami/camera.hpp"
#include "inugami/geometry.hpp"
#include "inugami/image.hpp"
#include "inugami/interface.hpp"
#include "inugami/mesh.hpp"
#include "inugami/transform.hpp"

using namespace Inugami;

MainMenu::MainMenu(Core& c)
    : core(c)
    , background(Image::fromPNG("data/img/mainmenu.png"), false, false)
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
    core.applyCam(Camera{}.ortho(-400.f, 400.f, -300.f, 300.f, -1.f, 1.f));
    core.modelMatrix(Transform{});

    background.bind(0);

    Mesh(Geometry::fromRect(800.f, 600.f)).draw();
}
