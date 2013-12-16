#include "hud.hpp"

#include "components.hpp"
#include "game.hpp"

#include "inugami/camera.hpp"
#include "inugami/geometry.hpp"
#include "inugami/image.hpp"
#include "inugami/mesh.hpp"
#include "inugami/texture.hpp"
#include "inugami/transform.hpp"

#include <sstream>

using namespace std;
using namespace Inugami;

HUD::HUD(Game& g)
    : game(g)
    , coinAnim()
    , font(Image::fromPNG("data/img/font.png"), 8, 8)
    , timerPulse(0)
{
    Spritesheet tmp (Image::fromPNG("data/img/coin.png"), 16, 16);

    coinAnim.setSpritesheet(move(tmp));
    coinAnim.setSprites({
        {0, 0} ,
        {0, 1} ,
        {0, 2} ,
        {0, 3} ,
        {0, 4} ,
        {0, 5} ,
        {0, 6} ,
        {0, 7} ,
    });
    coinAnim.setSequence({
        {0, 6} ,
        {1, 6} ,
        {2, 6} ,
        {3, 6} ,
        {4, 6} ,
        {5, 6} ,
        {6, 6} ,
        {7, 6} ,
    });
    coinAnim.setMode(AnimatedSprite::Mode::BOUNCE);
    coinAnim.scale = 0.5;
}

bool HUD::isOpaque() const
{
    return false;
}

bool HUD::isTunnel() const
{
    return true;
}

Screen::Event HUD::tick()
{
    coinAnim.tick();

    return {Event::Type::NONE, nullptr};
}

void HUD::draw()
{
    Camera cam;
    cam.ortho(-200.f, 200.f, -150.f, 150.f, -1.f, 1.f);
    game.core.applyCam(cam);

    Transform mat;

    mat.push();
    {
        mat.translate(0, 144.f);
        game.core.modelMatrix(mat);

        Texture black (Image(1, 1, {0, 0, 0, 255}), false, false);
        black.bind(0);

        Mesh panel (Geometry::fromRect(400.f, 12.f));
        panel.draw();
    }
    mat.pop();

    mat.push();
    {
        mat.translate(-194, 144);

        coinAnim.draw(game.core, mat);

        mat.translate(8, 0);

        stringstream ss;
        ss << game.player->getComponent<ECPlayer>()->gold;
        drawString(mat, ss.str());
    }
    mat.pop();

    mat.push();
    {
        mat.translate(186, 144);

        if (timerPulse > 0)
        {
            double scl = 1.f+(timerPulse/60.f);
            mat.scale(Vec3{scl, scl, 1.f});
            --timerPulse;
        }

        stringstream ss;
        ss << (game.timeRemaining/60);
        drawString(mat, ss.str());
    }
    mat.pop();
}

void HUD::drawString(Transform mat, const string& str)
{
    for (int c : str)
    {
        game.core.modelMatrix(mat);
        font.draw(c/16, c%16);
        mat.translate(8, 0);
    }
}

void HUD::pulseCounter()
{
    timerPulse = 60;
}
