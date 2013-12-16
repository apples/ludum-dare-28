#ifndef ECPOSITION_HPP
#define ECPOSITION_HPP

#include "entity.hpp"

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

void  adjustVelocity(ECPosition& a, const ECPosition& b);

#endif // ECPOSITION_HPP
