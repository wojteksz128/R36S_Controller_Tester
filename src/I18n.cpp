#include "I18n.h"

Language I18n::currentLanguage = Language::PL;

std::map<Language, std::map<std::string, std::string>> I18n::dictionary = {
    {Language::PL, {
        {"TITLE", "Tester Kontrolera R36S"},
        {"NO_PAD", "Brak sprzętu!"},
        {"PROFILE", "Profil sprzętowy (DB): "},
        {"EXIT_INSTR", "Wyjście: START + SELECT"},
        {"LANG_INSTR", "Zmiana języka: Y"},
        {"PRESSED_ID", "Wciśnięte ID: "},
        {"AXES_LOGICAL", "Osie logiczne (LX, LY): "}
    }},
    {Language::EN, {
        {"TITLE", "R36S Gamepad Tester"},
        {"NO_PAD", "No hardware!"},
        {"PROFILE", "Hardware Profile (DB): "},
        {"EXIT_INSTR", "Exit: START + SELECT"},
        {"LANG_INSTR", "Change lang: Y"},
        {"PRESSED_ID", "Pressed IDs: "},
        {"AXES_LOGICAL", "Logical axes (LX, LY): "}
    }}
};

void I18n::setLanguage(Language lang) {
    currentLanguage = lang;
}

void I18n::toggleLanguage() {
    currentLanguage = (currentLanguage == Language::PL) ? Language::EN : Language::PL;
}

std::string I18n::get(const std::string& key) {
    auto& dict = dictionary[currentLanguage];
    if (dict.find(key) != dict.end()) {
        return dict[key];
    }
    return key;
}