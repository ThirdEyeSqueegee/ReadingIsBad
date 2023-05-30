#include "Events.h"
#include "Utility.h"

namespace Events {
    OnSpellCastEventHandler* OnSpellCastEventHandler::GetSingleton() {
        static OnSpellCastEventHandler singleton;
        return &singleton;
    }

    RE::BSEventNotifyControl OnSpellCastEventHandler::ProcessEvent(const RE::TESSpellCastEvent* event,
                                                                   RE::BSTEventSource<RE::TESSpellCastEvent>* source) {
        if (!event) return RE::BSEventNotifyControl::kContinue;

        const auto casted_spell = RE::TESForm::LookupByID(event->spell);
        const auto spell_form = Utility::GetSingleton()->ReadSpell;

        if (const auto spell = casted_spell->As<RE::SpellItem>(); spell != spell_form)
            return RE::BSEventNotifyControl::kContinue;

        const auto game = RE::TES::GetSingleton();
        const auto player = RE::PlayerCharacter::GetSingleton();

        game->ForEachReferenceInRange(player, 256.0f, [&](RE::TESObjectREFR& ref) {
            const auto object = ref.GetBaseObject();
            if (object->GetFormType() == RE::FormType::Book) {
                const auto book = object->As<RE::TESObjectBOOK>();
                if (!book->IsRead()) book->Read(player);
            }

            return RE::BSContainer::ForEachResult::kContinue;
        });

        return RE::BSEventNotifyControl::kContinue;
    }

    void OnSpellCastEventHandler::Register() {
        RE::ScriptEventSourceHolder* holder = RE::ScriptEventSourceHolder::GetSingleton();
        holder->AddEventSink(GetSingleton());
        SKSE::log::info("Registered event handler");
    }
}
