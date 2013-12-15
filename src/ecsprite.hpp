#ifndef ECSPRITE_HPP
#define ECSPRITE_HPP

#include "entity.hpp"

#include "inugami/animatedsprite.hpp"

class ECSprite
    : public EntityComponent<ECSprite>
{
  public:
    std::map<std::string, Inugami::AnimatedSprite> anims;

    Inugami::AnimatedSprite* currentAnim = nullptr;
};

#endif // ECSPRITE_HPP
