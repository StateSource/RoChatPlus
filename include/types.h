#ifndef TYPES_H
#define TYPES_H

#include <QString>
#include <QDateTime>
#include <QVariant>
#include <vector>

// Message structure for chat communication
struct Message {
    QString id;
    QString sender;
    QString content;
    QDateTime timestamp;
    QString serverId;
    bool containsImage = false;
    bool containsLink = false;
    QStringList linkUrls;
    QString imageData;  // Base64 encoded or URL

    Message() = default;
    Message(const QString &senderId, const QString &msg)
        : sender(senderId), content(msg), timestamp(QDateTime::currentDateTime()) {}
};

// User information
struct User {
    QString id;
    QString name;
    QString avatar;
    QString status;  // "online", "away", "offline"
    QDateTime lastSeen;
};

// Server/Channel information
struct Server {
    QString id;
    QString name;
    QString description;
    std::vector<User> members;
    QDateTime createdAt;
    bool isConnected = false;
};

// Chat configuration
struct ChatConfig {
    int maxMessageLength = 1000;
    int minMessageLength = 1;
    float messageRefreshRate = 0.5f;
    int maxHistorySize = 100;
    int chatTimeout = 30;
    bool enableImageSharing = true;
    bool enableLinkSharing = true;
    bool enableAutoModeration = true;
};

// Network configuration
struct NetworkConfig {
    QString serverAddress;
    int port = 8443;
    bool useSSL = true;
    QString authToken;
    int reconnectAttempts = 5;
    int reconnectDelayMs = 3000;
};

#endif // TYPES_H
