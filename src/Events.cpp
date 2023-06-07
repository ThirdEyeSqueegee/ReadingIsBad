#include "Events.h"
#include "Utility.h"
#include "Settings.h"

namespace Events {
    OnSpellCastEventHandler* OnSpellCastEventHandler::GetSingleton() {
        static OnSpellCastEventHandler singleton;
        return std::addressof(singleton);
    }

    RE::BSEventNotifyControl OnSpellCastEventHandler::ProcessEvent(const RE::TESSpellCastEvent* event,
                                                                   RE::BSTEventSource<RE::TESSpellCastEvent>* source) {
        if (!event) return RE::BSEventNotifyControl::kContinue;

        const auto spell_form = Utility::GetSingleton()->ReadSpell;

        if (event->spell != spell_form->GetFormID())
            return RE::BSEventNotifyControl::kContinue;

        const auto game = RE::TES::GetSingleton();
        const auto player = RE::PlayerCharacter::GetSingleton();

        auto radius = Settings::GetSingleton()->radius;

        for (const auto& caster : player->GetActorRuntimeData().magicCasters)
            if (caster && caster->GetCasterAsActor() && caster->GetIsDualCasting()) radius *= 2;

        // https://www.creationkit.com/index.php?title=Unit
        game->ForEachReferenceInRange(player, radius, [&](RE::TESObjectREFR& ref) {
            const auto object = ref.GetBaseObject();
            if (object->GetFormType() == RE::FormType::Book) {
                if (const auto book = object->As<RE::TESObjectBOOK>(); !book->IsRead()) {
                    book->Read(player);
                    auto notif = std::format("Book read: {}", book->GetName());
                    if (book->TeachesSkill())
                        notif = std::format("Skill Book read: {}", book->GetName());
                    else if (book->TeachesSpell()) 
                        notif = std::format("Spell Tome read: {}", book->GetName());
                    RE::DebugNotification(notif.c_str());
                }
            } else if (object->GetFormType() == RE::FormType::Container) {
                for (const auto books = ref.GetInventory([](const RE::TESBoundObject& item) { return item.IsBook(); });
                     const auto obj : books | std::views::keys) {
                    const auto book = obj->As<RE::TESObjectBOOK>();
                    if (!book->IsRead()) book->Read(player);
                    auto notif = std::format("Book read: {}", book->GetName());
                    if (book->TeachesSkill())
                        notif = std::format("Skill Book read: {}", book->GetName());
                    else if (book->TeachesSpell()) 
                        notif = std::format("Spell Tome read: {}", book->GetName());
                    RE::DebugNotification(notif.c_str());
                }
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
