#include "Events.h"
#include "Utility.h"

namespace Events {
    OnSpellCastEventHandler* OnSpellCastEventHandler::GetSingleton() {
        static OnSpellCastEventHandler singleton;
        return std::addressof(singleton);
    }

    RE::BSEventNotifyControl OnSpellCastEventHandler::ProcessEvent(const RE::TESSpellCastEvent* event,
                                                                   RE::BSTEventSource<RE::TESSpellCastEvent>* source) {
        if (!event) return RE::BSEventNotifyControl::kContinue;

        const auto spell_form = Utility::GetSingleton()->ReadSpell;

        if (event->spell != spell_form->GetFormID()) {
            logger::info("Detected other spell");
            return RE::BSEventNotifyControl::kContinue;
        }

        const auto game = RE::TES::GetSingleton();
        const auto player = RE::PlayerCharacter::GetSingleton();

        auto radius = 256.0f;

        const auto caster = player->GetActorRuntimeData().magicCasters[0];

        for (const auto& caster : player->GetActorRuntimeData().magicCasters) {
            if (caster && caster->GetCasterAsActor() && caster->GetIsDualCasting()) radius *= 2;  
        }

        // https://www.creationkit.com/index.php?title=Unit
        game->ForEachReferenceInRange(player, radius, [&](RE::TESObjectREFR& ref) {
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
        const auto holder = RE::ScriptEventSourceHolder::GetSingleton();
        holder->AddEventSink(GetSingleton());
        logger::info("Registered event handler");
    }
}
