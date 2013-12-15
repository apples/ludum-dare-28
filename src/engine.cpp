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
#include "mainmenu.hpp"
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
    , screens()
{
    ScopedProfile prof(profiler, "CustomCore: Constructor");

    logger->log("Adding callbacks...");
    core.addCallback([this]{ tick(); draw(); }, 60.0);

    core.setWindowTitle("Ludum Dare 26", true);

    screens.push_back(ScreenPtr(new MainMenu(core)));
}

void Engine::go()
{
    core.go();
}

void Engine::tick()
{
    ScopedProfile prof(profiler, "CustomCore: Tick");

    core.iface->poll();

    if (core.shouldClose() || screens.empty())
    {
        core.running = false;
        return;
    }

    auto iter = screens.rbegin();
    bool tunnel = true;

    while (tunnel && iter != screens.rend())
    {
        auto ev = (*iter)->tick();

        switch (ev.type)
        {
            case Screen::Event::PUSH:
            {
                screens.push_back(ScreenPtr(ev.value));
                tunnel = false;
                continue;
            break;}

            case Screen::Event::POP:
            {
                screens.erase((++iter).base(), screens.end());
                tunnel = false;
                continue;
            break;}
        }

        tunnel = (*iter)->isTunnel();
    }
}

void Engine::draw()
{
    ScopedProfile prof(profiler, "CustomCore: Draw");

    if (!screens.empty())
    {
        core.beginFrame();

        auto first = (++find_if(
              screens.rbegin()
            , screens.rend()
            , [](const ScreenPtr& ptr){ return ptr->isOpaque(); }
        )).base();

        for (auto iter = first, eiter = screens.end(); iter!=eiter; ++iter)
        {
            (*iter)->draw();
        }

        core.endFrame();
    }
}
