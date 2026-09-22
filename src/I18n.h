#pragma once
#include <string>
#include <map>

enum class Language { PL, EN };

class I18n {
private:
    static Language currentLanguage;
    static std::map<Language, std::map<std::string, std::string>> dictionary;

public:
    static void setLanguage(Language language);
    static void toggleLanguage();
    static std::string get(const std::string& key);
};