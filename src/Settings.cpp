#include "Settings.h"

void Settings::LoadSettings() noexcept
{
    logger::info("Loading settings");

    CSimpleIniA ini;

    ini.SetUnicode();
    ini.LoadFile(R"(.\Data\SKSE\Plugins\ReadingIsBad.ini)");

    debug_logging = ini.GetBoolValue("Log", "Debug");

    if (debug_logging) {
        spdlog::set_level(spdlog::level::debug);
        logger::debug("Debug logging enabled");
    }

    radius     = static_cast<float>(ini.GetDoubleValue("General", "fSpellRange"));
    skip_tomes = ini.GetBoolValue("General", "bSkipSpellTomes");

    logger::info("Loaded settings");
    logger::info("\tfSpellRange = {}", radius);
    logger::info("\tSkipSpellTomes = {}", skip_tomes);
    logger::info("");
}
