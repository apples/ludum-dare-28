#ifndef ECPOSITION_HPP
#define ECPOSITION_HPP

#include "entity.hpp"

class ECPosition
    : public EntityComponent<ECPosition>
{
  public:
    double x;
    double y;
    double width;
    double height;
};

#endif // ECPOSITION_HPP
