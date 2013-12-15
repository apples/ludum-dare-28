#include "ecitem.hpp"

#include "ecplayer.hpp"

#include <map>
#include <functional>

using namespace std;

void ECItem::applyEffect(Entity* ent) const
{
    ECPlayer& player = *ent->getComponent<ECPlayer>();

    map<string, function<void()>> funcs = {
        {"hp5", [&]{player.hp += 5;}} ,
        {"gold5", [&]{player.gold += 5;}} ,
    };
}
