#pragma once
#include <Arduino.h>
#include <string_view>

enum class LogLevel {
    INFO = 0,
    WARN = 1,
    ERR = 2,
    NONE = 3 // 아무것도 안 찍을 때
};

class Logger {
public:
    // 생성 시 기본 레벨 설정
    explicit Logger(LogLevel level) : _currentLevel(level) {}

    // 운영 중에 레벨 변경 가능
    void setLevel(LogLevel level) { _currentLevel = level; }

    // 두 인자를 받는 로그 출력 함수
    void log(std::string_view level, std::string_view message) const;

private:
    LogLevel _currentLevel;
    int getLevelPriority(std::string_view level) const;
};

extern Logger logger;