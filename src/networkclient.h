#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <QObject>
#include <QWebSocket>
#include <QNetworkAccessManager>
#include <QQueue>
#include <memory>
#include "include/types.h"

class NetworkClient : public QObject
{
    Q_OBJECT

public:
    explicit NetworkClient(QObject *parent = nullptr);
    ~NetworkClient() override;

    bool connectToServer(const QString &address, int port);
    void disconnect();
    void sendMessage(const Message &message);
    void sendImage(const QString &serverId, const QByteArray &imageData);
    void sendLink(const QString &serverId, const QString &url);
    
    bool isConnected() const;
    const NetworkConfig &getConfig() const { return m_config; }

signals:
    void connected(const QString &serverId);
    void disconnected(const QString &serverId);
    void messageReceived(const Message &message);
    void connectionError(const QString &error);
    void imageReceived(const QString &serverId, const QByteArray &imageData);
    void linkValidationResult(const QString &url, bool isMalicious);

private slots:
    void onConnected();
    void onDisconnected();
    void onTextMessageReceived(const QString &message);
    void onBinaryMessageReceived(const QByteArray &data);
    void onError(QAbstractSocket::SocketError error);
    void onSslErrors(const QList<QSslError> &errors);

private:
    void parseMessage(const QString &data);
    void serializeMessage(const Message &message);
    void setupWebSocket();
    void reconnect();

    std::unique_ptr<QWebSocket> m_webSocket;
    std::unique_ptr<QNetworkAccessManager> m_networkManager;
    
    NetworkConfig m_config;
    QQueue<Message> m_messageQueue;
    
    bool m_isConnected = false;
    int m_reconnectAttempts = 0;
    int m_maxReconnectAttempts = 5;
};

#endif // NETWORKCLIENT_H
