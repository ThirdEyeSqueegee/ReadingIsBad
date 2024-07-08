#pragma once

#include "Utility.h"

namespace FormLookup
{
    static void LoadSpell()
    {
        const auto handler{ RE::TESDataHandler::GetSingleton() };
        Utility::read_spell = handler->LookupForm<RE::SpellItem>(0x800, "ReadingIsBad.esp"sv);
        logger::info("Loaded ReadSpell: {}", Utility::read_spell->GetName());
    }
} // namespace FormLookup
