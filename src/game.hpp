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

    int previousScore;

    Game(Inugami::Core& c, const std::string& file, int ps);

    virtual bool isOpaque() const override;
    virtual bool isTunnel() const override;

    virtual Event tick() override;
    virtual void draw() override;

    void resetPlayer();
};

#endif // GAME_HPP
