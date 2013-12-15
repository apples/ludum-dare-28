#ifndef ECCOLLISION_HPP
#define ECCOLLISION_HPP

#include "entity.hpp"

class ECCollision
    : public EntityComponent<ECCollision>
{
  public:
    double width;
    double height;
};

#endif // ECCOLLISION_HPP
