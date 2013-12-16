#include "game.hpp"

#include "components.hpp"
#include "level.hpp"
#include "meta.hpp"
#include "random.hpp"

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
    , hud(nullptr)
{
    player->addComponent<ECPosition>();
    player->addComponent<ECSprite>();
    player->addComponent<ECCollision>();
    player->addComponent<ECPlayer>();

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
    if (keyUp)    pos.dy+=1.0;
    if (keyDown)  pos.dy-=1.0;
    if (keyLeft)  pos.dx-=1.0;
    if (keyRight) pos.dx+=1.0;

    auto&& walls = level.getEntities<ECPosition, ECSolid>();

    {
        auto&& ents = level.getEntities<ECPosition>();
        for (auto&& tup : ents)
        {
            Entity& entity = *get<0>(tup);
            ECPosition& ent = *get<1>(tup);

            bool collide = entity.getComponent<ECCollision>();

            if (collide) for (auto&& tup2 : walls)
            {
                if (get<0>(tup) == get<0>(tup2)) continue;

                Entity& entity2 = *get<0>(tup2);
                ECPosition& ent2 = *get<1>(tup2);

                bool solid = entity2.getComponent<ECSolid>();

                if (solid)
                {
                    adjustVelocity(ent, ent2);
                }
            }

            ent.x += ent.dx;
            ent.y += ent.dy;
            ent.dx *= 1.0 - ent.friction;
            ent.dy *= 1.0 - ent.friction;
            ent.dx += ent.bounce * ent.cx;
            ent.dy += ent.bounce * ent.cy;
            ent.cx = 0.0;
            ent.cy = 0.0;
        }
    }

    {
        auto&& items = level.getEntities<ECPosition, ECItem>();
        for (auto&& tup : items)
        {
            Entity* entity = get<0>(tup);
            ECPosition& ipos = *get<1>(tup);
            ECItem& item = *get<2>(tup);

            if (--item.cooldown < 0)
            {
                double mk_left = ipos.x - ipos.width/2.0 - pos.x - pos.width/2.0;
                double mk_right = mk_left + ipos.width + pos.width;

                double mk_bottom = ipos.y - ipos.height/2.0 - pos.y - pos.height/2.0;
                double mk_top = mk_bottom + ipos.height + pos.height;

                if (mk_left < 0.0 && mk_right > 0.0
                 && mk_bottom < 0.0 && mk_top > 0.0)
                {
                    item.applyEffect(entity, player);
                }
            }
        }
    }

    static int cnt = 0;

    if (keySpace.pressed())
    {
        int r = (pos.y-pos.height/3.0)/32.0+0.5;
        int c = pos.x/32.0+0.5;
        auto& tile = level.tileAt(r, c);

        if (tile != 1)
        {
            tile = 1;

            auto& itemname = level.itemAt(r, c);

            if (!itemname.empty() && itemname != "~")
            {
                logger->log("You found a ", itemname);

                Entity* item = level.newEntity();

                ECPosition& cpos = *item->addComponent<ECPosition>();
                ECSprite& csprite = *item->addComponent<ECSprite>();
                ECItem& citem = *item->addComponent<ECItem>();
                *item->addComponent<ECCollision>();

                citem.effect = itemname;

                cpos.x = c*32.0;
                cpos.y = r*32.0;
                cpos.dx = 5.0 * Random::roll(-1.0, 1.0);
                cpos.dy = 5.0 * Random::roll(-1.0, 1.0);
                cpos.friction = 0.05;
                cpos.bounce = 1.0;
                cpos.width = 8.0;
                cpos.height = 8.0;

                auto& csheet = csprite.anims["item"];

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

        if (++cnt == 5)
        {
            cnt = 0;

        }
    }

    level.eraseEntities<ECDestroy>();

    if (!hud)
    {
        hud = new HUD(core, player);
        return {Event::PUSH, hud};
    }

    return {Event::NONE, nullptr};
}

void Game::draw()
{
    const auto& pos = *player->getComponent<ECPosition>();

    Camera cam;
    cam.ortho(-200.f, 200.f, -150.f, 150.f, -1.f, 1.f);
    cam.translate(Vec3{-int(pos.x), -int(pos.y)-6, 0.f});

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
        mat.translate(int(get<2>(ent)->x), int(get<2>(ent)->y));
        anim->draw(core, mat);
        mat.pop();
    }
}
