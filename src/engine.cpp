/*******************************************************************************
 * Inugami - An OpenGL framework designed for rapid game development
 * Version: 0.3.0
 * https://github.com/DBRalir/Inugami
 *
 * Copyright (c) 2012 Jeramy Harrison <dbralir@gmail.com>
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the
 * use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software
 *     in a product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 *
 *  2. Altered source versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

#include "engine.hpp"
#include "audiodevice.hpp"

#include "meta.hpp"

#include "inugami/camera.hpp"
#include "inugami/geometry.hpp"
#include "inugami/image.hpp"
#include "inugami/interface.hpp"
#include "inugami/loaders.hpp"
#include "inugami/math.hpp"
#include "inugami/shader.hpp"
#include "inugami/shaderprogram.hpp"
#include "inugami/transform.hpp"
#include "inugami/utility.hpp"

#include <fstream>
#include <utility>

using namespace Inugami;
using namespace irrklang;

Engine::Engine(const RenderParams &params)
    : core(params)
{
    ScopedProfile prof(profiler, "CustomCore: Constructor");

    logger->log("Adding callbacks...");
    core.addCallback([this]{ tick(); draw(); }, 60.0);

    core.setWindowTitle("Ludum Dare 26", true);

    auto&& adev = AudioDevice::inst();
    auto tst = adev.loadSource("data/fart.ogg");
    adev.play(tst);
}

void Engine::tick()
{
    ScopedProfile prof(profiler, "CustomCore: Tick");

    auto iface = core.iface.get().get();

    //Keybinds can be stored in proxies (efficient!)
    auto keyW     = iface->key('W');
    auto keyA     = iface->key('A');
    auto keyS     = iface->key('S');
    auto keyD     = iface->key('D');
    auto keyZ     = iface->key('Z');
    auto keyX     = iface->key('X');
    auto keyC     = iface->key('C');
    auto keySpace = iface->key(' ');
    auto keyESC   = iface->key(0_ivkFunc);
    auto keyF1    = iface->key(1_ivkFunc);
    auto keyF2    = iface->key(2_ivkFunc);
    auto keyF5    = iface->key(5_ivkFunc);

    //Poll must be called every frame
    iface->poll();

    auto mousePos = iface->getMousePos();

    //Key Proxies can be cast to bool
    if (keyESC || core.shouldClose())
    {
        core.running = false;
        return;
    }
}

void Engine::draw()
{
    ScopedProfile prof(profiler, "CustomCore: Draw");

    //beginFrame() sets the OpenGL context to the proper initial state
    core.beginFrame();

    //endFrame() swaps the buffer to the screen
    core.endFrame();
}
