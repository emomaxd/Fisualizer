#pragma once

#include "../Renderer/Window.hpp"

#include <vector>

class Engine{

public:
    Engine();
    ~Engine();

    void start();
    

private:
    Window* window;
};