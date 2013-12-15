#ifndef ECITEM_HPP
#define ECITEM_HPP

#include "entity.hpp"

#include <string>

class ECItem
    : public EntityComponent<ECItem>
{
  public:
    std::string effect;
    int cooldown = 30;

    void applyEffect(Entity* player) const;
};

#endif // ECITEM_HPP
