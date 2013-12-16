#include "level.hpp"

#include "components.hpp"
#include "entity.hpp"
#include "meta.hpp"
#include "random.hpp"

#include <yaml-cpp/yaml.h>

#include <exception>
#include <functional>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
using namespace Inugami;

Level::Level()
    : tiles()
    , items()
    , width(0)
    , height(0)
    , entities()
    , nextLevel()
{}

Level::Level(const string& filename)
    : tiles()
    , items()
    , width()
    , height()
    , entities()
    , nextLevel()
{
    YAML::Node file = YAML::LoadFile(filename);

    auto&& nlvl = file["next"];
    if (nlvl) nextLevel = nlvl.as<string>();

    width  = file["width"] .as<int>();
    height = file["height"].as<int>();

    tiles.resize(height,  Row(width));
    items.resize(height, IRow(width));

    bool decorate = file["decorate"];
    function<int(int)> prettify = [](int i){return i;};
    mt19937 rng;

    if (decorate)
    {
        rng.seed(file["decorate"].as<int>());

        prettify = [&](int i)
        {
            uniform_int_distribution<int> dist (0,2);
            uniform_int_distribution<int> dist2 (0,1);
            if (dist(rng) == 0)
            {
                if (dist2(rng) == 0)
                {
                    return 32;
                }
                else
                {
                    return 33;
                }
            }
            return 0;
        };
    }

    for (int i=0; i<height; ++i)
    {
        for (int j=0; j<width; ++j)
        {
            auto&& node = file["items"][i][j];
            if (node) items[i][j] = node.as<Item>();

            tiles[i][j] = file["tiles"][i][j].as<int>();

            switch (tiles[i][j])
            {
                case 0:
                {
                    tiles[i][j] = prettify(tiles[i][j]);
                break;}

                case 16:
                {
                    Entity* ent = newEntity();
                    ent->addComponent<ECSolid>();
                    ECPosition* pos = ent->addComponent<ECPosition>();

                    pos->x = j*32.0;
                    pos->y = i*32.0;
                    pos->width = 32.0;
                    pos->height = 32.0;
                break;}
            }
        }
    }

    string componentName;

    for (auto&& node : file["entities"])
    {
        logger->log("Found entity");

        Entity* ent = newEntity();

        for (auto&& comp : node)
        {
            componentName = comp.first.as<string>();
            logger->log("Found component ", componentName);

            unordered_map<string, function<void()>> funcs = {
                { "position", [&]()
                    {
                        ECPosition* ptr = ent->addComponent<ECPosition>();

                        ptr->x = comp.second["x"].as<double>();
                        ptr->y = comp.second["y"].as<double>();

                        auto&& nw = comp.second["width"];
                        auto&& nh = comp.second["height"];

                        if (nw) ptr->width  = nw.as<double>();
                        if (nh) ptr->height = nh.as<double>();
                    }
                } ,
                { "collision", [&]()
                    {
                        ent->addComponent<ECCollision>();
                    }
                } ,
                { "solid", [&]()
                    {
                        ent->addComponent<ECSolid>();
                    }
                } ,
                { "sprite", [&]()
                    {
                        ECSprite* ptr = ent->addComponent<ECSprite>();

                        for (auto&& anim : comp.second)
                        {
                            string animName = anim.first.as<string>();
                            string imgName = anim.second["image"].as<string>();
                            int w = anim.second["width"].as<int>();
                            int h = anim.second["height"].as<int>();

                            // nomral, loop, bounce
                            using M = AnimatedSprite::Mode;
                            M m = M(anim.second["mode"].as<int>());

                            AnimatedSprite& sprite = ptr->anims[animName];

                            Spritesheet sheet(Image::fromPNG(imgName), w, h);
                            sprite.setSpritesheet(move(sheet));

                            AnimatedSprite::SpriteList sl;
                            AnimatedSprite::FrameList fl;
                            for (auto&& frm : anim.second["frames"])
                            {
                                int r = frm["row"].as<int>();
                                int c = frm["col"].as<int>();
                                int d = frm["duration"].as<int>();
                                sl.push_back({r,c});
                                fl.push_back({sl.size()-1, d});
                            }

                            sprite.setSprites(sl);
                            sprite.setSequence(fl);

                            sprite.setMode(m);
                        }
                    }
                } ,
            };

            auto iter = funcs.find(componentName);
            if (iter != funcs.end()) iter->second();
        }
    }
}

Level::Tile& Level::tileAt(int r, int c)
{
    return tiles.at(r).at(c);
}

Level::Item& Level::itemAt(int r, int c)
{
    return items.at(r).at(c);
}

int Level::getWidth() const
{
    return width;
}

int Level::getHeight() const
{
    return height;
}

void Level::setWidth(int w)
{
    width = w;
    for (auto&& row : tiles)
    {
        row.resize(width, 0);
    }
}

void Level::setHeight(int h)
{
    height = h;
    tiles.resize(height, Row(width, 0));
}

Entity* Level::newEntity()
{
    Entity* ent = new Entity;
    entities.push_back(unique_ptr<Entity>(ent));
    return ent;
}
