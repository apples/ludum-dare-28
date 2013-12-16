#ifndef GAME_HPP
#define GAME_HPP

#include "screen.hpp"

#include "level.hpp"

#include "inugami/core.hpp"
#include "inugami/spritesheet.hpp"

#include <string>

class HUD;

class Game
    : public Screen
{
public:
    Inugami::Core& core;

    Inugami::Spritesheet tiles;

    Level level;

    Entity* player;

    HUD* hud;

    int timeRemaining;

    Game(Inugami::Core& c, const std::string& file);

    virtual bool isOpaque() const override;
    virtual bool isTunnel() const override;

    virtual Event tick() override;
    virtual void draw() override;

    void resetPlayer();
};

#endif // GAME_HPP
