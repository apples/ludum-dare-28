#ifndef ECPLAYER_HPP
#define ECPLAYER_HPP

#include "entity.hpp"

class ECPlayer
    : public EntityComponent<ECPlayer>
{
  public:
    int gold = 0;
    int hp = 100;
};

#endif // ECPLAYER_HPP
