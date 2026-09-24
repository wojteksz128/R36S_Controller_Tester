#include "ConfigParser.h"
#include <fstream>
#include <sstream>
#include <vector>

std::map<std::string, Mapping> ConfigParser::parseDB(const std::string& guid, const std::string& name) {
    std::map<std::string, Mapping> map;
    std::string targetLine = "";
    std::ifstream file("/opt/inttools/gamecontrollerdb.txt");

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (line.rfind(guid, 0) == 0) {
                targetLine = line;
                break;
            }
        }
        if (targetLine.empty() && !name.empty()) {
            file.clear();
            file.seekg(0);
            while (std::getline(file, line)) {
                if (line.find("," + name + ",") != std::string::npos) {
                    targetLine = line;
                    break;
                }
            }
        }
    }

    if (targetLine.empty()) {
        targetLine = "190000004b4800000011000000010000,GO-Super Gamepad,a:b1,b:b0,x:b2,y:b3,leftshoulder:b4,rightshoulder:b5,dpdown:b9,dpleft:b10,dpright:b11,dpup:b8,leftx:a0,lefty:a1,rightx:a2,righty:a3,leftstick:b14,rightstick:b15,lefttrigger:b6,righttrigger:b7,back:b12,start:b13,";
    }

    std::stringstream ss(targetLine);
    std::string token;
    std::vector<std::string> parts;
    while (std::getline(ss, token, ',')) {
        parts.push_back(token);
    }

    for (size_t i = 2; i < parts.size(); ++i) {
        size_t colonPos = parts[i].find(':');
        if (colonPos == std::string::npos) continue;
        
        std::string logical = parts[i].substr(0, colonPos);
        std::string physical = parts[i].substr(colonPos + 1);
        
        Mapping m;
        if (!physical.empty() && physical.back() == '~') {
            m.invert = true;
            physical.pop_back();
        }

        if (physical[0] == 'b') {
            m.type = InputType::BUTTON;
            m.id = std::stoi(physical.substr(1));
        } else if (physical[0] == 'a') {
            m.type = InputType::AXIS;
            m.id = std::stoi(physical.substr(1));
        } else if (physical[0] == 'h') {
            m.type = InputType::HAT;
            size_t dotPos = physical.find('.');
            if (dotPos != std::string::npos) {
                m.id = std::stoi(physical.substr(1, dotPos - 1));
                m.direction = std::stoi(physical.substr(dotPos + 1));
            }
        }
        map[logical] = m;
    }
    return map;
}