#pragma once

namespace Events
{
    class OnSpellCastEventHandler : public EventSingleton<OnSpellCastEventHandler, RE::TESSpellCastEvent>
    {
    public:
        RE::BSEventNotifyControl ProcessEvent(const RE::TESSpellCastEvent* a_event, RE::BSTEventSource<RE::TESSpellCastEvent>* a_eventSource) noexcept override;
    };
} // namespace Events
