#pragma once

class Debug {
public:
    [[nodiscard]] spdlog::level::level_enum GetLogLevel() const noexcept {
        return _logLevel;
    }

    [[nodiscard]] spdlog::level::level_enum GetFlushLevel() const noexcept {
        return _flushLevel;
    }

    void Deserialize(std::istream& in);

private:
    spdlog::level::level_enum _logLevel{spdlog::level::level_enum::info};
    spdlog::level::level_enum _flushLevel{spdlog::level::level_enum::trace};
};

class Config {
public:
    [[nodiscard]] const Debug& GetDebug() const noexcept {
        return _debug;
    }

    [[nodiscard]] static Config* GetSingleton() noexcept;

    void Deserialize(std::istream& in);

private:
    Debug _debug;
};
