#pragma once

namespace Events {
    class OnSpellCastEventHandler : public RE::BSTEventSink<RE::TESSpellCastEvent> {
    public:
        static OnSpellCastEventHandler* GetSingleton();

        RE::BSEventNotifyControl ProcessEvent(const RE::TESSpellCastEvent* event,
                                              RE::BSTEventSource<RE::TESSpellCastEvent>* source) override;

        static void Register();
    };
}
