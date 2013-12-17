#ifndef GAMEOVER_HPP
#define GAMEOVER_HPP

#include "screen.hpp"

#include "inugami/core.hpp"

class Game;

class GameOver
    : public Screen
{
    Inugami::Core& core;

public:
    GameOver(Inugami::Core& c);

    virtual bool isOpaque() const override;
    virtual bool isTunnel() const override;

    virtual Event tick() override;
    virtual void draw() override;
};

#endif // GAMEOVER_HPP
