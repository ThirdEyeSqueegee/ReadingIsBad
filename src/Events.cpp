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
                    auto notif = std::format("Book read: {}", book->GetName());
                    if (book->TeachesSkill())
                        notif = std::format("Book read: {} (Skill)", book->GetName());
                    else if (book->TeachesSpell()) {
                        notif = std::format("Book read: {} (Spell)", book->GetName());
                        const auto actor = player->As<RE::Actor>();
                        if (book->CanBeTaken()) {
                            actor->PickUpObject(&ref, 1);
                            player->RemoveItem(book, 1, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);
                        }
                    }
                    RE::DebugNotification(notif.c_str());
                    book->Read(player);
                }
            } else if (object->GetFormType() == RE::FormType::Container) {
                const auto books = ref.GetInventory([](const RE::TESBoundObject& item) { return item.IsBook(); });
                for (const auto obj : books | std::views::keys) {
                    const auto book = obj->As<RE::TESObjectBOOK>();
                    auto notif = std::format("Book read: {}", book->GetName());
                    if (book->TeachesSkill())
                        notif = std::format("Book read: {} (Skill)", book->GetName());
                    else if (book->TeachesSpell()) {
                        notif = std::format("Book read: {} (Spell)", book->GetName());
                        const auto actor = player->As<RE::Actor>();
                        if (book->CanBeTaken()) {
                            actor->PickUpObject(book->AsReference(), 1);
                            player->RemoveItem(book, 1, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);
                        }
                    }
                    RE::DebugNotification(notif.c_str());
                    book->Read(player);
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
