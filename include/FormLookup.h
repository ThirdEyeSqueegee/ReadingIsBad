#pragma once

#include "Utility.h"

namespace FormLookup {
    static void LoadSpell() {
        const auto handler = RE::TESDataHandler::GetSingleton();
        const auto utility = Utility::GetSingleton();
        utility->ReadSpell = handler->LookupForm(0x800, "ReadingIsBad.esp")->As<RE::SpellItem>();
        SKSE::log::info("Loaded ReadSpell: {}", utility->ReadSpell->GetName());
    }
}
