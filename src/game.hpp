#ifndef GAME_HPP
#define GAME_HPP

#include "screen.hpp"

#include "hud.hpp"
#include "level.hpp"

#include "inugami/core.hpp"
#include "inugami/spritesheet.hpp"

class Game
    : public Screen
{
    Inugami::Core& core;

    Inugami::Spritesheet tiles;

    Level level;

    Entity* player;

    HUD* hud;

public:
    Game(Inugami::Core& c);

    virtual bool isOpaque() const override;
    virtual bool isTunnel() const override;

    virtual Event tick() override;
    virtual void draw() override;

    void resetPlayer();
};

#endif // GAME_HPP
