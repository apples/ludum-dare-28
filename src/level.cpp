#include "level.hpp"

#include "components.hpp"
#include "entity.hpp"
#include "meta.hpp"

#include <yaml-cpp/yaml.h>

#include <exception>
#include <functional>
#include <map>
#include <string>
#include <vector>

using namespace std;
using namespace Inugami;

Level::Level()
    : tiles()
    , width(0)
    , height(0)
    , entities()
{}

Level::Level(const string& filename)
    : tiles()
    , width()
    , height()
    , entities()
{
    YAML::Node file = YAML::LoadFile(filename);

    width  = file["width"] .as<int>();
    height = file["height"].as<int>();

    tiles.resize(height, Row(width));

    for (int i=0; i<height; ++i)
    {
        for (int j=0; j<width; ++j)
        {
            tiles[i][j] = file["tiles"][i][j].as<int>();
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

            map<string, function<void()>> funcs = {
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
                        ECCollision* ptr = ent->addComponent<ECCollision>();
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
