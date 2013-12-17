#ifndef SUCCESS_HPP
#define SUCCESS_HPP

#include "screen.hpp"

#include "inugami/core.hpp"
#include "inugami/spritesheet.hpp"
#include "inugami/texture.hpp"

class Game;

class Success
    : public Screen
{
    Inugami::Core& core;

    Inugami::Spritesheet font;
    Inugami::Texture bg;

    int score;

public:
    Success(Inugami::Core& c, int s);

    virtual bool isOpaque() const override;
    virtual bool isTunnel() const override;

    virtual Event tick() override;
    virtual void draw() override;

    void drawString(Inugami::Transform mat, const std::string& str);
};

#endif // SUCCESS_HPP
