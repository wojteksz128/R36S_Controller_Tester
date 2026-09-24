#pragma once
#include <string>
#include <map>

enum class InputType { BUTTON, AXIS, HAT, UNKNOWN };

struct Mapping {
    InputType type = InputType::UNKNOWN;
    int id = -1;
    int direction = 0;
    bool invert = false;
};

class ConfigParser {
public:
    static std::map<std::string, Mapping> parseDB(const std::string& guid, const std::string& name);
};