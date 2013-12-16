#ifndef HUD_HPP
#define HUD_HPP

#include "screen.hpp"

#include "entity.hpp"

#include "inugami/animatedsprite.hpp"
#include "inugami/core.hpp"
#include "inugami/spritesheet.hpp"
#include "inugami/transform.hpp"

#include <string>

class Game;

class HUD
    : public Screen
{
    Game& game;

    Inugami::AnimatedSprite coinAnim;
    Inugami::Spritesheet    font;

public:
    HUD(Game& g);

    virtual bool isOpaque() const override;
    virtual bool isTunnel() const override;

    virtual Event tick() override;
    virtual void draw() override;

    void drawString(Inugami::Transform mat, const std::string& str);
};

#endif // HUD_HPP
