#include "game.hpp"

<<<<<<< HEAD
#include "ecitem.hpp"
=======
#include "eccollision.hpp"
>>>>>>> Collision Detection Added
#include "ecposition.hpp"
#include "ecsprite.hpp"
#include "level.hpp"
#include "meta.hpp"

#include "inugami/camera.hpp"
#include "inugami/geometry.hpp"
#include "inugami/image.hpp"
#include "inugami/interface.hpp"
#include "inugami/mesh.hpp"
#include "inugami/transform.hpp"

#include <tuple>

using namespace std;
using namespace Inugami;

Game::Game(Core& c)
    : core(c)
    , tiles(Image::fromPNG("data/img/tiles.png"), 32, 32)
    , level("data/lvl/level1.lvl")
    , player(level.newEntity())
{
    player->addComponent<ECPosition>();
    player->addComponent<ECSprite>();
    player->addComponent<ECCollision>();

    resetPlayer();
}

void Game::resetPlayer()
{
    auto& pos = *player->getComponent<ECPosition>();
    auto& sprite = *player->getComponent<ECSprite>();

    pos.x = 64;
    pos.y = 64;
    pos.width = 24;
    pos.height = 24;

    auto& sheet = sprite.anims["walk"];

    sheet.setSpritesheet(Spritesheet(Image::fromPNG("data/img/chest1.png"), 24, 24));
    sheet.setSprites({
        {0, 0} ,
        {0, 1} ,
        {0, 2} ,
        {0, 3} ,
        {0, 4} ,
        {0, 5} ,
    });
    sheet.setSequence({
        {0, 10} ,
        {1, 10} ,
        {2, 10} ,
        {3, 10} ,
        {4, 10} ,
        {5, 10} ,
    });
    sheet.setMode(AnimatedSprite::Mode::BOUNCE);

    sprite.currentAnim = &sheet;
}

bool Game::isOpaque() const
{
    return true;
}

bool Game::isTunnel() const
{
    return false;
}

Screen::Event Game::tick()
{
    auto keyUp = core.iface->key('U'_ivkArrow);
    auto keyDown = core.iface->key('D'_ivkArrow);
    auto keyLeft = core.iface->key('L'_ivkArrow);
    auto keyRight = core.iface->key('R'_ivkArrow);
    auto keySpace = core.iface->key(' '_ivk);
    auto keyESC = core.iface->key(0_ivkFunc);

    if (keyESC.pressed()) return {Event::POP, this};

    for (auto&& ent : level.getEntities<ECSprite>())
    {
        auto& anim = get<1>(ent)->currentAnim;
        if (!anim)
        {
            anim = &( get<1>(ent)->anims.begin()->second );
        }
        anim->tick();
    }

    auto& pos = *player->getComponent<ECPosition>();
    pos.dx=0;
    pos.dy=0;
    if (keyUp)    pos.dy+=1;
    if (keyDown)  pos.dy-=1;
    if (keyLeft)  pos.dx-=1;
    if (keyRight) pos.dx+=1;

    {
        auto&& ents = level.getEntities<ECPosition>();
        for (auto iter = begin(ents), ei = end(ents); iter!=ei; ++iter)
        {
            auto& tup = *iter;
            Entity& entity = *get<0>(tup);
            ECPosition& ent = *get<1>(tup);

            ECCollision* collide = entity.getComponent<ECCollision>();

            if (collide) for (auto iter2 = iter+1; iter2 != ei; ++iter2)
            {
                auto& tup2 = *iter2;
                Entity& entity2 = *get<0>(tup);
                ECPosition& ent2 = *get<1>(tup2);

                ECCollision* collide2 = entity.getComponent<ECCollision>();

                if (collide2)
                {
                    adjustVelocity(ent, ent2);
                    adjustVelocity(ent2, ent);
                }

            }
            ent.x += ent.dx;
            ent.y += ent.dy;

        }
    }

    static int cnt = 0;

    if (keySpace.pressed())
    {
        int r = (pos.y-pos.height/3.0)/32.0+0.5;
        int c = pos.x/32.0+0.5;
        level.tileAt(r, c) = 1;

        if (++cnt == 5)
        {
            cnt = 0;

            Entity* coin = level.newEntity();

            ECPosition& cpos = *coin->addComponent<ECPosition>();
            ECSprite& csprite = *coin->addComponent<ECSprite>();
            ECItem& citem = *coin->addComponent<ECItem>();

            cpos.x = c*32.0;
            cpos.y = r*32.0;
            cpos.width = 8.0;
            cpos.height = 8.0;

            auto& csheet = csprite.anims["walk"];

            Spritesheet ctmp (Image::fromPNG("data/img/coin.png"), 24, 24);

            csheet.setSpritesheet(move(ctmp));
            csheet.setSprites({
                {0, 0} ,
                {0, 1} ,
                {0, 2} ,
                {0, 3} ,
                {0, 4} ,
                {0, 5} ,
            });
            csheet.setSequence({
                {0, 10} ,
                {1, 10} ,
                {2, 10} ,
                {3, 10} ,
                {4, 10} ,
                {5, 10} ,
            });
            csheet.setMode(AnimatedSprite::Mode::BOUNCE);

            csprite.currentAnim = &csheet;

        }
    }

    return {Event::NONE, nullptr};
}

void Game::draw()
{
    const auto& pos = *player->getComponent<ECPosition>();

    Camera cam;
    cam.ortho(-200.f, 200.f, -150.f, 150.f, -1.f, 1.f);
    cam.translate(Vec3{-pos.x, -pos.y, 0.f});

    core.applyCam(cam);

    Transform mat;

    mat.push();
    for (int r=0, re=level.getHeight(); r!=re; ++r)
    {
        mat.push();
        for (int c=0, ce=level.getWidth(); c!=ce; ++c)
        {
            core.modelMatrix(mat);
            int tile = level.tileAt(r, c);
            tiles.draw(tile/16, tile%16);
            mat.translate(32, 0);
        }
        mat.pop();
        mat.translate(0, 32);
    }
    mat.pop();

    mat.translate(0, 0, 0.1f);

    auto&& ents = level.getEntities<ECSprite, ECPosition>();
    for (auto&& ent : ents)
    {
        auto& anim = get<1>(ent)->currentAnim;
        if (!anim)
        {
            anim = &( get<1>(ent)->anims.begin()->second );
        }

        mat.push();
        mat.translate(get<2>(ent)->x, get<2>(ent)->y);
        anim->draw(core, mat);
        mat.pop();
    }
}
