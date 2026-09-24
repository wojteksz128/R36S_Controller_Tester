#include "App.h"
#include "I18n.h"
#include <iostream>

void App::run() {
    if (!gfx.init()) {
        std::cerr << "Blad inicjalizacji silnika" << std::endl;
        return;
    }
    input.init();
    
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            else input.handleEvent(e);
        }

        processInput();
        render();
        SDL_Delay(16); // ~60 FPS
    }
}

void App::processInput() {
    if (input.getState("start") && input.getState("back")) {
        running = false;
    }

    bool yPressed = input.getState("y");
    if (yPressed && !yWasPressed) {
        I18n::toggleLanguage();
    }
    yWasPressed = yPressed;
}

void App::render() {
    gfx.clear();

    // UI Top
    gfx.drawText(I18n::get("TITLE"), 20, 10, {255, 200, 100, 255}, false);
    gfx.drawText(I18n::get("PROFILE") + input.getJoyName(), 20, 35, {150, 150, 150, 255});
    gfx.drawText(I18n::get("EXIT_INSTR") + "  |  " + I18n::get("LANG_INSTR"), 20, 55, {100, 200, 255, 255});

    // Triggery i bumpery
    gfx.drawRectBtn(80, 80, 80, 25, input.getState("lefttrigger"), "L2");
    gfx.drawRectBtn(80, 115, 80, 25, input.getState("leftshoulder"), "L1");
    gfx.drawRectBtn(640 - 160, 80, 80, 25, input.getState("righttrigger"), "R2");
    gfx.drawRectBtn(640 - 160, 115, 80, 25, input.getState("rightshoulder"), "R1");

    // D-PAD
    int dpx = 150, dpy = 230;
    gfx.drawRectBtn(dpx - 15, dpy - 45, 30, 30, input.getState("dpup"), "U");
    gfx.drawRectBtn(dpx - 15, dpy + 15, 30, 30, input.getState("dpdown"), "D");
    gfx.drawRectBtn(dpx - 45, dpy - 15, 30, 30, input.getState("dpleft"), "L");
    gfx.drawRectBtn(dpx + 15, dpy - 15, 30, 30, input.getState("dpright"), "R");

    // Funkcyjne
    gfx.drawRectBtn(260, 280, 40, 15, input.getState("back"), "SEL");
    gfx.drawRectBtn(340, 280, 40, 15, input.getState("start"), "STA");

    // Akcje
    int bx = 490, by = 230;
    gfx.drawCircleBtn(bx, by + 35, 18, input.getState("b"), "B");
    gfx.drawCircleBtn(bx + 35, by, 18, input.getState("a"), "A");
    gfx.drawCircleBtn(bx - 35, by, 18, input.getState("y"), "Y");
    gfx.drawCircleBtn(bx, by - 35, 18, input.getState("x"), "X");

    // Analogi
    float lx = input.getAxis("leftx"), ly = input.getAxis("lefty");
    gfx.drawAnalog(210, 350, lx, ly, input.getState("leftstick"));
    gfx.drawAnalog(430, 350, input.getAxis("rightx"), input.getAxis("righty"), input.getState("rightstick"));

    // Dolny pasek diagnostyczny
    gfx.drawText(I18n::get("PRESSED_ID") + input.getRawButtonsStr(), 20, 410, {220, 220, 220, 255});
    std::string axesTxt = std::to_string(lx).substr(0,5) + ", " + std::to_string(ly).substr(0,5);
    gfx.drawText(I18n::get("AXES_LOGICAL") + axesTxt, 20, 440, {220, 220, 220, 255});
    
    gfx.present();
}