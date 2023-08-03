#pragma once

#include "Utility.h"

namespace FormLookup {
    static void LoadSpell() {
        const auto handler = RE::TESDataHandler::GetSingleton();
        const auto utility = Utility::GetSingleton();
        utility->read_spell = handler->LookupForm<RE::SpellItem>(0x800, "ReadingIsBad.esp");
        logger::info("Loaded ReadSpell: {}", utility->read_spell->GetName());
    }
}
