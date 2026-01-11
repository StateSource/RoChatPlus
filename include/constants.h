#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>

namespace Constants {
    // Application information
    constexpr const char* APP_NAME = "RoChat+";
    constexpr const char* APP_VERSION = "1.0.0";
    constexpr const char* APP_AUTHOR = "RoChat+ Team";
    
    // Message limits
    constexpr int MAX_MESSAGE_LENGTH = 1000;
    constexpr int MIN_MESSAGE_LENGTH = 1;
    constexpr int MAX_HISTORY_SIZE = 100;
    
    // Network settings
    constexpr int DEFAULT_PORT = 8443;
    constexpr int RECONNECT_ATTEMPTS = 5;
    constexpr int RECONNECT_DELAY_MS = 3000;
    constexpr int CONNECTION_TIMEOUT_MS = 30000;
    
    // UI settings
    constexpr int CHAT_REFRESH_RATE_MS = 500;
    constexpr int FONT_SIZE_DEFAULT = 11;
    
    // Image and link handling
    constexpr int MAX_IMAGE_SIZE_MB = 10;
    constexpr int MAX_IMAGE_DIMENSION = 4096;
    
    // Moderation settings
    constexpr float MALICIOUS_LINK_THRESHOLD = 0.8f;
    constexpr int LINK_CHECK_TIMEOUT_MS = 5000;
    
    // File paths
    const QString CONFIG_PATH = "RoChatPlus.ini";
    const QString BLACKLIST_PATH = "data/blacklist.txt";
    const QString LOG_PATH = "logs/";
}

#endif // CONSTANTS_H
