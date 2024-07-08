#pragma once

class Settings : public Singleton<Settings>
{
public:
    static void LoadSettings() noexcept;

    inline static bool debug_logging{};

    inline static bool skip_tomes{};

    inline static float radius{ 512.0f };
};
