#include "InputManager.h"
#include "I18n.h"
#include <cmath>

InputManager::InputManager() : joyName(I18n::get("NO_PAD")) {}

InputManager::~InputManager() {
    if (joystick) {
        SDL_JoystickClose(joystick);
    }
}

void InputManager::init() {
    if (SDL_NumJoysticks() > 0) {
        joystick = SDL_JoystickOpen(0);
        if (joystick) {
            joyName = SDL_JoystickName(joystick);
            char guidStr[64];
            SDL_JoystickGetGUIDString(SDL_JoystickGetGUID(joystick), guidStr, sizeof(guidStr));
            logicalMap = ConfigParser::parseDB(guidStr, joyName);
        }
    }
}

void InputManager::handleEvent(const SDL_Event& e) {
    if (e.type == SDL_JOYBUTTONDOWN) rawButtons.insert(e.jbutton.button);
    else if (e.type == SDL_JOYBUTTONUP) rawButtons.erase(e.jbutton.button);
    else if (e.type == SDL_JOYAXISMOTION) {
        float val = e.jaxis.value / 32767.0f;
        rawAxes[e.jaxis.axis] = (std::abs(val) > 0.15f) ? val : 0.0f;
    }
    else if (e.type == SDL_JOYHATMOTION) rawHats[e.jhat.hat] = e.jhat.value;
}

bool InputManager::getState(const std::string& name) const {
    auto it = logicalMap.find(name);
    if (it == logicalMap.end()) return false;
    
    const Mapping& m = it->second;
    bool isPressed = false;

    if (m.type == InputType::BUTTON) {
        isPressed = rawButtons.count(m.id) > 0;
    } else if (m.type == InputType::HAT) {
        int hatVal = rawHats.count(m.id) ? rawHats.at(m.id) : SDL_HAT_CENTERED;
        if (m.direction == 1 && (hatVal & SDL_HAT_UP)) isPressed = true;
        else if (m.direction == 4 && (hatVal & SDL_HAT_DOWN)) isPressed = true;
        else if (m.direction == 8 && (hatVal & SDL_HAT_LEFT)) isPressed = true;
        else if (m.direction == 2 && (hatVal & SDL_HAT_RIGHT)) isPressed = true;
    } else if (m.type == InputType::AXIS) {
        float val = rawAxes.count(m.id) ? rawAxes.at(m.id) : 0.0f;
        isPressed = (val > 0.5f);
    }
    return m.invert ? !isPressed : isPressed;
}

float InputManager::getAxis(const std::string& name) const {
    auto it = logicalMap.find(name);
    if (it == logicalMap.end() || it->second.type != InputType::AXIS) return 0.0f;
    
    float val = rawAxes.count(it->second.id) ? rawAxes.at(it->second.id) : 0.0f;
    return it->second.invert ? -val : val;
}

std::string InputManager::getJoyName() const {
    return joyName;
}

std::string InputManager::getRawButtonsStr() const {
    if (rawButtons.empty()) return "brak";
    std::string res = "";
    for (int b : rawButtons) res += std::to_string(b) + ", ";
    return res.substr(0, res.length() - 2);
}

bool InputManager::isButtonPressed(int id) const {
    return rawButtons.count(id) > 0;
}