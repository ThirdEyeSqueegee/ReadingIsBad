#include "Events.h"

#include "Settings.h"
#include "Utility.h"

namespace Events {
    OnSpellCastEventHandler* OnSpellCastEventHandler::GetSingleton() {
        static OnSpellCastEventHandler singleton;
        return std::addressof(singleton);
    }

    RE::BSEventNotifyControl OnSpellCastEventHandler::ProcessEvent(const RE::TESSpellCastEvent* a_event,
                                                                   RE::BSTEventSource<RE::TESSpellCastEvent>* a_eventSource) {
        if (!a_event)
            return RE::BSEventNotifyControl::kContinue;

        if (const auto spell_form = Utility::read_spell; a_event->spell != spell_form->GetFormID())
            return RE::BSEventNotifyControl::kContinue;

        const auto game = RE::TES::GetSingleton();
        const auto player = RE::PlayerCharacter::GetSingleton();

        auto radius = Settings::radius;
        auto is_dual_casting = false;
        player->GetGraphVariableBool("IsCastingDual"sv, is_dual_casting);
        if (is_dual_casting)
            radius *= 2;

        game->ForEachReferenceInRange(player, radius, [&](RE::TESObjectREFR& ref) {
            if (const auto object = ref.GetBaseObject(); object->IsBook()) {
                if (const auto book = object->As<RE::TESObjectBOOK>(); !book->IsRead() && !book->IsNoteScroll()) {
                    auto notif = std::format("Book read: {}", book->GetName());
                    if (book->TeachesSkill()) {
                        logger::debug("Reading skill book {}", book->GetName());
                        notif = std::format("Skill Book read: {}", book->GetName());
                        book->Read(player);
                        RE::DebugNotification(notif.c_str());
                    } else if (book->TeachesSpell() && !player->HasSpell(book->GetSpell()) && !Settings::skip_tomes) {
                        logger::debug("Reading spell tome {}", book->GetName());
                        notif = std::format("Spell Tome read: {}", book->GetName());
                        book->Read(player);
                        RE::DebugNotification(notif.c_str());
                    } else if (!book->TeachesSkill() && !book->TeachesSpell()) {
                        logger::debug("Reading book {}", book->GetName());
                        book->Read(player);
                        RE::DebugNotification(notif.c_str());
                    }
                }
            } else if (object->GetFormType() == RE::FormType::Container) {
                logger::debug("Container {} detected", object->GetName());
                if (const auto container_name = std::string_view(object->GetName()); !container_name.contains("Chest"sv)) {
                    for (const auto books = ref.GetInventory([](const RE::TESBoundObject& item) { return item.IsBook(); });
                         const auto obj : books | std::views::keys) {
                        if (const auto book = obj->As<RE::TESObjectBOOK>(); !book->IsRead() && !book->IsNoteScroll()) {
                            auto notif = std::format("Book read: {}", book->GetName());
                            if (book->TeachesSkill()) {
                                logger::debug("Reading skill book {}", book->GetName());
                                notif = std::format("Skill Book read: {}", book->GetName());
                                book->Read(player);
                                RE::DebugNotification(notif.c_str());
                            } else if (book->TeachesSpell() && !player->HasSpell(book->GetSpell()) && !Settings::skip_tomes) {
                                logger::debug("Reading spell tome {}", book->GetName());
                                notif = std::format("Spell Tome read: {}", book->GetName());
                                book->Read(player);
                                RE::DebugNotification(notif.c_str());
                            } else if (!book->TeachesSkill() && !book->TeachesSpell()) {
                                logger::debug("Reading book {}", book->GetName());
                                book->Read(player);
                                RE::DebugNotification(notif.c_str());
                            }
                        }
                    }
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
