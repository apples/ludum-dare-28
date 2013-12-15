#ifndef BANK_HPP
#define BANK_HPP

#include "inugami/texture.hpp"

#include <map>
#include <memory>
#include <string>

class Bank
{
    std::map<std::string, std::unique_ptr<Inugami::Texture>> textures;

  public:
    Inugami::Texture* getTexture(const std::string& filename);

    void clear();
};

#endif // BANK_HPP
