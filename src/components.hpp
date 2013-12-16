#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include "entity.hpp"

#include "inugami/animatedsprite.hpp"

#include <map>
#include <string>

class ECCollision
    : public EntityComponent<ECCollision>
{};

class ECSolid
    : public EntityComponent<ECSolid>
{};

class ECItem
    : public EntityComponent<ECItem>
{
  public:
    std::string effect;
    int cooldown = 30;

    void applyEffect(Entity* player) const;
};

class ECPlayer
    : public EntityComponent<ECPlayer>
{
  public:
    int gold = 0;
    int hp = 100;
};

class ECPosition
    : public EntityComponent<ECPosition>
{
  public:
    double x;
    double y;
    double dx=0;
    double dy=0;
    double width;
    double height;
};

void adjustVelocity(ECPosition& a, const ECPosition& b);

class ECSprite
    : public EntityComponent<ECSprite>
{
  public:
    std::map<std::string, Inugami::AnimatedSprite> anims;

    Inugami::AnimatedSprite* currentAnim = nullptr;
};

#endif // COMPONENTS_HPP
