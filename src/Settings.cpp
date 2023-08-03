#include "Settings.h"

#include "SimpleIni.h"

Settings* Settings::GetSingleton() {
    static Settings singleton;
    return std::addressof(singleton);
}

void Settings::LoadSettings() {
    logger::info("Loading settings");

    CSimpleIniA ini;

    ini.SetUnicode();
    ini.LoadFile(R"(.\Data\SKSE\Plugins\ReadingIsBad.ini)");

    debug_logging = ini.GetBoolValue("Log", "Debug");

    if (debug_logging) {
        spdlog::get("Global")->set_level(spdlog::level::level_enum::debug);
        logger::debug("Debug logging enabled");
    }

    radius = static_cast<float>(ini.GetDoubleValue("General", "fSpellRange"));
    skip_tomes = ini.GetBoolValue("General", "bSkipSpellTomes");

    logger::info("Loaded settings");
    logger::info("\tfSpellRange = {}", radius);
    logger::info("\tSkipSpellTomes = {}", skip_tomes);
}
