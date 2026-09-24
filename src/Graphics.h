#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class Graphics {
private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    TTF_Font* fontSmall = nullptr;
    TTF_Font* fontMedium = nullptr;
    
    SDL_Color bgColor = {25, 25, 30, 255};
    SDL_Color outlineColor = {150, 150, 150, 255};
    SDL_Color activeColor = {50, 255, 100, 255};
    SDL_Color textColor = {220, 220, 220, 255};

    void drawCircleRaw(int x0, int y0, int radius, SDL_Color color, bool filled);

public:
    Graphics();
    ~Graphics();

    bool init();
    void clear();
    void present();
    
    void drawText(const std::string& text, int x, int y, SDL_Color color, bool useSmallFont = true);
    void drawRectBtn(int x, int y, int w, int h, bool isActive, const std::string& label);
    void drawCircleBtn(int x, int y, int radius, bool isActive, const std::string& label);
    void drawAnalog(int baseX, int baseY, float axisX, float axisY, bool isPressed);
};