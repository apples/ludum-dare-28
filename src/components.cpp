#include "components.hpp"

#include "audiodevice.hpp"

#include "inugami/math.hpp"

#include <map>
#include <functional>
#include <string>

using namespace std;
using namespace Inugami;

void ECItem::applyEffect(Entity* item, Entity* ent) const
{
    ECPlayer& player = *ent->getComponent<ECPlayer>();

    map<string, function<void()>> funcs = {
        {"gold", [&]
            {
                player.gold += param;
                item->addComponent<ECDestroy>();
                AudioDevice::inst().quickPlay("data/sfx/coin.wav");
            }
        } ,
    };

    funcs[effect]();
}

void adjustVelocity(ECPosition& a, const ECPosition& b)
{
    struct Bounds
    {
        double left;
        double right;
        double bottom;
        double top;
    };

    auto make_bounds = [](const ECPosition& p)
    {
        Bounds rval;
        rval.left   = p.x - p.width/2.0;
        rval.bottom = p.y - p.height/2.0;
        rval.right = rval.left + p.width;
        rval.top   = rval.bottom + p.height;
        return rval;
    };

    Bounds a_bounds = make_bounds(a);
    Bounds b_bounds = make_bounds(b);

    using MP = double ECPosition::*;
    using BP = double Bounds::*;
    auto adjust = [&](MP loc, MP vel, MP wid, MP c, BP bot, BP lft, BP rgt)
    {
        if (a.*vel != 0.0)
        {
            double m_bottom = b_bounds.*bot - a_bounds.*bot - a.*wid;
            double m_top = m_bottom + a.*wid + b.*wid;

            if (m_bottom < 0.0 && m_top > 0.0)
            {
                if (a.*vel > 0.0)
                {
                    double new_right = a_bounds.*rgt + a.*vel;
                    if (new_right > b_bounds.*lft && new_right < b_bounds.*rgt)
                    {
                        double new_vel = b_bounds.*lft - a_bounds.*rgt;
                        if (sgn(new_vel) != sgn(a.*vel)) new_vel = 0.0;
                        a.*c += new_vel - a.*vel;
                        a.*vel = new_vel;
                    }
                }
                else
                {
                    double new_left = a_bounds.*lft + a.*vel;
                    if (new_left < b_bounds.*rgt && new_left > b_bounds.*lft)
                    {
                        double new_vel = b_bounds.*rgt - a_bounds.*lft;
                        if (sgn(new_vel) != sgn(a.*vel)) new_vel = 0.0;
                        a.*c += new_vel - a.*vel;
                        a.*vel = new_vel;
                    }
                }
            }
        }
    };

    adjust(
          &ECPosition::x, &ECPosition::dx, &ECPosition::height, &ECPosition::cx
        , &Bounds::bottom, &Bounds::left, &Bounds::right
    );

    adjust(
          &ECPosition::y, &ECPosition::dy, &ECPosition::width, &ECPosition::cy
        , &Bounds::left, &Bounds::bottom, &Bounds::top
    );
}
