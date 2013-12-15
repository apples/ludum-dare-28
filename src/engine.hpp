#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "screen.hpp"

#include "inugami/core.hpp"

#include "inugami/animatedsprite.hpp"
#include "inugami/mesh.hpp"
#include "inugami/shader.hpp"
#include "inugami/spritesheet.hpp"
#include "inugami/texture.hpp"

#include <memory>
#include <vector>

class Engine
{
    using ScreenPtr = std::unique_ptr<Screen>;

    Inugami::Core core;

    std::vector<ScreenPtr> screens;
public:
    using RenderParams = Inugami::Core::RenderParams;

    Engine(const RenderParams& params);

    void go();

    void tick();
    void draw();
};

#endif // ENGINE_HPP
