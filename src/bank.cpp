#include "bank.hpp"

using namespace std;
using namespace Inugami;

Texture* Bank::getTexture(const string& filename)
{
    auto iter = textures.find(filename);
    if (iter != textures.end())
    {
        return iter->second.get();
    }
    //iter =
}
