#include "Settings.h"
#include "SimpleIni.h"
#include "Logging.h"

Settings* Settings::GetSingleton() {
    static Settings singleton;
    return std::addressof(singleton);
}

void Settings::LoadSettings() {
    logger::info("Loading settings");

    CSimpleIniA ini;

    ini.SetUnicode();
    ini.LoadFile(R"(.\Data\SKSE\Plugins\ReadingIsBad.ini)");

    radius = static_cast<float>(std::atof(ini.GetValue("General", "fSpellRange")));

    logger::info("Loaded settings");
}
