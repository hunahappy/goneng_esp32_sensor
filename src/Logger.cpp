#include "Logger.h"

int Logger::getLevelPriority(std::string_view level) const {
    if (level == "info") return 0;
    if (level == "warn") return 1;
    if (level == "err")  return 2;
    return -1; // 지정되지 않은 로그는 무조건 출력(-1은 0보다 작으므로)
}

void Logger::log(std::string_view level, std::string_view message) const {
    int msgPriority = getLevelPriority(level);
    
    if (static_cast<int>(_currentLevel) > msgPriority && msgPriority != -1) {
        return;
    }

    // [수정된 부분]
    // string_view는 문자열 길이를 알고 있으므로, 생성자에 길이 정보를 같이 전달해야 안전합니다.
    String levelStr = String(level.data(), level.size()); 
    levelStr.toUpperCase();
    
    Serial.printf("[%s] %s\n", levelStr.c_str(), message.data());
}