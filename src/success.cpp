#include "success.hpp"

#include "inugami/camera.hpp"
#include "inugami/geometry.hpp"
#include "inugami/image.hpp"
#include "inugami/interface.hpp"
#include "inugami/mesh.hpp"
#include "inugami/texture.hpp"

#include <sstream>

using namespace std;
using namespace Inugami;

Success::Success(Inugami::Core& c, int s)
    : core(c)
    , font(Image::fromPNG("data/img/font.png"), 8, 8)
    , score(s)
{}

bool Success::isOpaque() const
{
    return true;
}

bool Success::isTunnel() const
{
    return false;
}

Screen::Event Success::tick()
{
    auto keyESC = core.iface->key(0_ivkFunc);

    if (keyESC.pressed()) return {Event::POP, this};

    return {Event::NONE, nullptr};
}

void Success::draw()
{
    core.applyCam(Camera{}.ortho(-400.f, 400.f, -300.f, 300.f, -1.f, 1.f));
    core.modelMatrix(Transform{});

    Texture bg (Image::fromPNG("data/img/success.png"), false, false);
    bg.bind(0);

    Mesh(Geometry::fromRect(800.f, 600.f)).draw();

    stringstream ss;
    ss << score;

    Transform mat;
    mat.scale(Vec3{2.f, 2.f, 1.f});
    drawString(mat, "Score:");
    mat.translate(56, 0);
    drawString(mat, ss.str());
}

void Success::drawString(Transform mat, const string& str)
{
    for (int c : str)
    {
        core.modelMatrix(mat);
        font.draw(c/16, c%16);
        mat.translate(8, 0);
    }
}
