#include "Graphics.h"

Graphics::Graphics() {}

Graphics::~Graphics() {
    if (fontSmall) TTF_CloseFont(fontSmall);
    if (fontMedium) TTF_CloseFont(fontMedium);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

bool Graphics::init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) return false;
    if (TTF_Init() == -1) return false;

    window = SDL_CreateWindow("R36S Tester", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    if (!window) return false;
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) return false;
    
    fontMedium = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 20);
    fontSmall = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 16);
    
    return fontMedium && fontSmall;
}

void Graphics::drawCircleRaw(int x0, int y0, int radius, SDL_Color color, bool filled) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx*dx + dy*dy) <= (radius * radius)) {
                if (filled || (dx*dx + dy*dy) >= ((radius-2) * (radius-2))) {
                    SDL_RenderDrawPoint(renderer, x0 + dx, y0 + dy);
                }
            }
        }
    }
}

void Graphics::clear() {
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderClear(renderer);
}

void Graphics::present() {
    SDL_RenderPresent(renderer);
}

void Graphics::drawText(const std::string& text, int x, int y, SDL_Color color, bool useSmallFont) {
    TTF_Font* f = useSmallFont ? fontSmall : fontMedium;
    if (!f || text.empty()) return;
    SDL_Surface* surf = TTF_RenderUTF8_Blended(f, text.c_str(), color);
    if (!surf) return;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_Rect rect = {x, y, surf->w, surf->h};
    SDL_RenderCopy(renderer, tex, nullptr, &rect);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
}

void Graphics::drawRectBtn(int x, int y, int w, int h, bool isActive, const std::string& label) {
    SDL_Rect rect = {x, y, w, h};
    SDL_Color c = isActive ? activeColor : outlineColor;
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    
    if (isActive) SDL_RenderFillRect(renderer, &rect);
    else SDL_RenderDrawRect(renderer, &rect);
    
    drawText(label, x + w/2 - 8, y + h/2 - 8, isActive ? SDL_Color{0,0,0,255} : textColor);
}

void Graphics::drawCircleBtn(int x, int y, int radius, bool isActive, const std::string& label) {
    drawCircleRaw(x, y, radius, isActive ? activeColor : outlineColor, isActive);
    drawText(label, x - 6, y - 8, isActive ? SDL_Color{0,0,0,255} : textColor);
}

void Graphics::drawAnalog(int baseX, int baseY, float axisX, float axisY, bool isPressed) {
    drawCircleRaw(baseX, baseY, 40, outlineColor, false);
    int stickX = baseX + static_cast<int>(axisX * 25);
    int stickY = baseY + static_cast<int>(axisY * 25);
    drawCircleRaw(stickX, stickY, 20, isPressed ? activeColor : SDL_Color{180,180,180,255}, true);
}