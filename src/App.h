#pragma once
#include "Graphics.h"
#include "InputManager.h"

class App {
private:
    Graphics gfx;
    InputManager input;
    bool running = true;
    bool yWasPressed = false;

    void processInput();
    void render();

public:
    void run();
};