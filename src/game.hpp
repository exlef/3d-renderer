#pragma once

#include "ex3d/app.hpp"
#include "ex3d/input.hpp"

class Game
{
private:
    ex::App app = ex::App(800, 600, "test");
public:
    Game();
    ~Game();
};
