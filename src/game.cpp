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

    pos.x = level.getWidth()*16.0-16.0;
    pos.y = level.getHeight()*16.0-16.0;
    pos.width = 24;
    pos.height = 24;

    {
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
            {0, 6} ,
            {1, 6} ,
            {2, 6} ,
            {3, 6} ,
            {4, 6} ,
            {5, 6} ,
        });
        sheet.setMode(AnimatedSprite::Mode::BOUNCE);

        sprite.currentAnim = &sheet;
    }

    {
        auto& sheet = sprite.anims["dig"];

        sheet.setSpritesheet(Spritesheet(Image::fromPNG("data/img/chest1.png"), 24, 24));
        sheet.setSprites({
            {0, 0} ,
            {0, 2} ,
            {0, 5} ,
        });
        sheet.setSequence({
            {0, 1} ,
            {1, 1} ,
            {2, 1} ,
        });
        sheet.setMode(AnimatedSprite::Mode::BOUNCE);
    }

    {
        auto& sheet = sprite.anims["idle"];

        sheet.setSpritesheet(Spritesheet(Image::fromPNG("data/img/chest1.png"), 24, 24));
        sheet.setSprites({
            {0, 0} ,
        });
        sheet.setSequence({
            {0, 1} ,
        });
        sheet.setMode(AnimatedSprite::Mode::LOOP);
    }
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

    ECPosition& pos = *player->getComponent<ECPosition>();
    ECPlayer* p = player->getComponent<ECPlayer>();

    if (p->digTime > 0)
    {
        --p->digTime;

        int r = (pos.y-pos.height/3.0)/32.0+0.5;
        int c = pos.x/32.0+0.5;
        auto& tile = level.tileAt(r, c);

        if (tile != 1 && p->digTime < 20)
        {
            tile = 1;

            auto& itemname = level.itemAt(r, c);

            if (!itemname.empty() && itemname != "~")
            {
                stringstream ss (itemname);
                int param;
                ss >> itemname >> param;

                Entity* item = level.newEntity();

                ECPosition& cpos = *item->addComponent<ECPosition>();
                ECSprite& csprite = *item->addComponent<ECSprite>();
                ECItem& citem = *item->addComponent<ECItem>();
                *item->addComponent<ECCollision>();

                citem.effect = itemname;
                citem.param = param;

                cpos.x = c*32.0;
                cpos.y = r*32.0;
                cpos.dx = 5.0 * Random::roll(-1.0, 1.0);
                cpos.dy = 5.0 * Random::roll(-1.0, 1.0);
                cpos.friction = 0.05;
                cpos.bounce = 1.0;

                cpos.width = 8.0;
                cpos.height = 8.0;

                if (itemname == "gold")
                {
                    cpos.width *= (param+5)/10.0;
                    cpos.height *= (param+5)/10.0;
                }

                auto& csheet = csprite.anims["item"];

                Spritesheet ctmp (Image::fromPNG("data/img/coin.png"), 16, 16);

                csheet.setSpritesheet(move(ctmp));
                csheet.setSprites({
                    {0, 0} ,
                    {0, 1} ,
                    {0, 2} ,
                    {0, 3} ,
                    {0, 4} ,
                    {0, 5} ,
                    {0, 6} ,
                    {0, 7} ,
                });
                csheet.setSequence({
                    {0, 6} ,
                    {1, 6} ,
                    {2, 6} ,
                    {3, 6} ,
                    {4, 6} ,
                    {5, 6} ,
                    {6, 6} ,
                    {7, 6} ,
                });
                csheet.setMode(AnimatedSprite::Mode::BOUNCE);
                csheet.scale = 0.5;

                csprite.currentAnim = &csheet;
            }
        }
    }
    else
    {
        ECSprite* ps = player->getComponent<ECSprite>();
        ps->currentAnim = &ps->anims["idle"];
        bool wlk = false;
        if (keyUp)
        {
            pos.dy+=1.0;
            wlk = true;
        }
        if (keyDown)
        {
            pos.dy-=1.0;
            wlk = true;
        }
        if (keyLeft)
        {
            pos.dx-=1.0;
            wlk = true;
        }
        if (keyRight)
        {
            pos.dx+=1.0;
            wlk = true;
        }
        if (wlk) ps->currentAnim = &ps->anims["walk"];
    }

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

    if (keySpace.pressed())
    {
        int r = (pos.y-pos.height/3.0)/32.0+0.5;
        int c = pos.x/32.0+0.5;
        auto& tile = level.tileAt(r, c);

        if (tile != 1)
        {
            {
                p->digTime = 60;
                ECSprite* ps = player->getComponent<ECSprite>();
                ps->currentAnim = &ps->anims["dig"];
                ps->currentAnim->reset();
            }
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

        ECItem* item = get<0>(ent)->getComponent<ECItem>();
        if (item && item->effect == "gold")
        {
            double scl = get<2>(ent)->width/8.0;
            mat.scale(Vec3{scl, scl, 1.0});
        }

        anim->draw(core, mat);
        mat.pop();
    }
}
