#pragma once

class Utility {
public:
    RE::SpellItem* ReadSpell;

    static Utility* GetSingleton() {
        static Utility singleton;
        return &singleton;
    }
};
