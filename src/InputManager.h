#pragma once
#include <SDL.h>
#include <string>
#include <set>
#include <map>
#include "ConfigParser.h"

class InputManager {
private:
    std::set<int> rawButtons;
    std::map<int, float> rawAxes;
    std::map<int, int> rawHats;
    std::map<std::string, Mapping> logicalMap;
    SDL_Joystick* joystick = nullptr;
    std::string joyName;

public:
    InputManager();
    ~InputManager();

    void init();
    void handleEvent(const SDL_Event& e);
    
    bool getState(const std::string& name) const;
    float getAxis(const std::string& name) const;
    
    std::string getJoyName() const;
    std::string getRawButtonsStr() const;
    bool isButtonPressed(int id) const;
};