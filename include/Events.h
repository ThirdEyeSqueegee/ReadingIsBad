#pragma once

namespace Events {
    class OnSpellCastEventHandler : public RE::BSTEventSink<RE::TESSpellCastEvent> {
    protected:
        OnSpellCastEventHandler() = default;

    public:
        OnSpellCastEventHandler(const OnSpellCastEventHandler&) = delete;
        OnSpellCastEventHandler(OnSpellCastEventHandler&&) = delete;
        OnSpellCastEventHandler& operator=(const OnSpellCastEventHandler&) = delete;
        OnSpellCastEventHandler& operator=(OnSpellCastEventHandler&) = delete;

        static OnSpellCastEventHandler* GetSingleton();
        RE::BSEventNotifyControl ProcessEvent(const RE::TESSpellCastEvent* event,
                                              RE::BSTEventSource<RE::TESSpellCastEvent>* source) override;
        static void Register();
    };
}
