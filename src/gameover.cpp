#include "gameover.hpp"

#include "inugami/camera.hpp"
#include "inugami/geometry.hpp"
#include "inugami/image.hpp"
#include "inugami/interface.hpp"
#include "inugami/mesh.hpp"
#include "inugami/texture.hpp"

using namespace Inugami;

GameOver::GameOver(Inugami::Core& c)
    : core(c)
{}

bool GameOver::isOpaque() const
{
    return true;
}

bool GameOver::isTunnel() const
{
    return false;
}

Screen::Event GameOver::tick()
{
    auto keyESC = core.iface->key(0_ivkFunc);

    if (keyESC.pressed()) return {Event::POP, this};

    return {Event::NONE, nullptr};
}

void GameOver::draw()
{
    core.applyCam(Camera{}.ortho(-400.f, 400.f, -300.f, 300.f, -1.f, 1.f));
    core.modelMatrix(Transform{});

    Texture bg (Image::fromPNG("data/img/gameover.png"), false, false);
    bg.bind(0);

    Mesh(Geometry::fromRect(800.f, 600.f)).draw();
}
