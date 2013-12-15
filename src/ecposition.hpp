#ifndef ECPOSITION_HPP
#define ECPOSITION_HPP

#include "entity.hpp"

class ECPosition
    : public EntityComponent<ECPosition>
{
  public:
    double x;
    double y;
};

#endif // ECPOSITION_HPP
