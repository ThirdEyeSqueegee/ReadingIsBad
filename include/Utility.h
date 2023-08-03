#pragma once

class Utility {
protected:
    Utility() = default;
    ~Utility() = default;

public:
    Utility(const Utility&) = delete;
    Utility(Utility&&) = delete;
    Utility& operator=(const Utility&) = delete;
    Utility& operator=(Utility&&) = delete;

    static Utility* GetSingleton() {
        static Utility singleton;
        return std::addressof(singleton);
    }

    inline static RE::SpellItem* read_spell;
};
