#include "Events.h"

#include "Settings.h"
#include "Utility.h"

namespace Events
{
    RE::BSEventNotifyControl OnSpellCastEventHandler::ProcessEvent(const RE::TESSpellCastEvent* a_event, RE::BSTEventSource<RE::TESSpellCastEvent>* a_eventSource) noexcept
    {
        if (!a_event) {
            return RE::BSEventNotifyControl::kContinue;
        }

        if (const auto spell_form = Utility::read_spell; a_event->spell != spell_form->GetFormID()) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const auto player{ RE::PlayerCharacter::GetSingleton() };
        const auto cell{ player->GetParentCell() };

        auto radius{ Settings::radius };
        bool is_dual_casting{};
        player->GetGraphVariableBool("IsCastingDual"sv, is_dual_casting);
        if (is_dual_casting) {
            radius *= 2;
        }

        cell->ForEachReferenceInRange(player->GetPosition(), radius, [&](RE::TESObjectREFR* ref) {
            if (const auto object{ ref->GetBaseObject() }; object && object->IsBook()) {
                if (const auto book{ object->As<RE::TESObjectBOOK>() }; book && !book->IsRead() && !book->IsNoteScroll()) {
                    const auto book_name{ book->GetName() };
                    if (book->TeachesSkill()) {
                        logger::debug("Reading skill book {}", book_name);
                        book->Read(player);
                        RE::DebugNotification(fmt::format("Skill book read: {}", book_name).c_str());
                    }
                    else if (book->TeachesSpell() && !player->HasSpell(book->GetSpell()) && !Settings::skip_tomes) {
                        logger::debug("Reading spell tome {}", book_name);
                        book->Read(player);
                        RE::DebugNotification(std::format("Spell Tome read: {}", book_name).c_str());
                    }
                    else if (!book->TeachesSkill() && !book->TeachesSpell()) {
                        logger::debug("Reading book {}", book_name);
                        book->Read(player);
                        RE::DebugNotification(fmt::format("Book read: {}", book_name).c_str());
                    }
                }
            }
            else if (object && object->GetFormType() == RE::FormType::Container) {
                const auto obj_name{ object->GetName() };
                logger::debug("Container {} detected", obj_name);
                if (const auto container_name{ std::string_view(obj_name) }; !container_name.contains("Chest"sv)) {
                    for (const auto books{ ref->GetInventory([](const RE::TESBoundObject& item) { return item.IsBook(); }) }; const auto obj : books | std::views::keys) {
                        if (const auto book{ obj->As<RE::TESObjectBOOK>() }; book && !book->IsRead() && !book->IsNoteScroll()) {
                            const auto book_name{ book->GetName() };
                            if (book->TeachesSkill()) {
                                logger::debug("Reading skill book {}", book_name);
                                book->Read(player);
                                RE::DebugNotification(fmt::format("Skill book read: {}", book_name).c_str());
                            }
                            else if (book->TeachesSpell() && !player->HasSpell(book->GetSpell()) && !Settings::skip_tomes) {
                                logger::debug("Reading spell tome {}", book_name);
                                book->Read(player);
                                RE::DebugNotification(fmt::format("Spell Tome read: {}", book_name).c_str());
                            }
                            else if (!book->TeachesSkill() && !book->TeachesSpell()) {
                                logger::debug("Reading book {}", book_name);
                                book->Read(player);
                                RE::DebugNotification(fmt::format("Book read: {}", book_name).c_str());
                            }
                        }
                    }
                }
            }
            return RE::BSContainer::ForEachResult::kContinue;
        });

        return RE::BSEventNotifyControl::kContinue;
    }
} // namespace Events
