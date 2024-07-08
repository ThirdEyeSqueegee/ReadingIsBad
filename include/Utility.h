#pragma once

class Utility : public Singleton<Utility>
{
public:
    inline static RE::SpellItem* read_spell{};
};
