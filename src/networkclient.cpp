#include "networkclient.h"
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QTimer>
#include <QSslConfiguration>

NetworkClient::NetworkClient(QObject *parent)
    : QObject(parent)
    , m_webSocket(std::make_unique<QWebSocket>())
    , m_networkManager(std::make_unique<QNetworkAccessManager>(this))
{
    setupWebSocket();
}

NetworkClient::~NetworkClient()
{
    if (m_isConnected) {
        disconnect();
    }
}

bool NetworkClient::connectToServer(const QString &address, int port)
{
    m_config.serverAddress = address;
    m_config.port = port;
    
    QString url = QString("wss://%1:%2").arg(address).arg(port);
    
    qDebug() << "Connecting to:" << url;
    m_webSocket->open(QUrl(url));
    
    return true;
}

void NetworkClient::disconnect()
{
    if (m_webSocket) {
        m_webSocket->close();
        m_isConnected = false;
    }
}

void NetworkClient::sendMessage(const Message &message)
{
    if (!m_isConnected) {
        qWarning() << "Not connected to server, queueing message";
        m_messageQueue.enqueue(message);
        return;
    }
    
    QJsonObject jsonMessage;
    jsonMessage["type"] = "message";
    jsonMessage["sender"] = message.sender;
    jsonMessage["content"] = message.content;
    jsonMessage["serverId"] = message.serverId;
    jsonMessage["timestamp"] = message.timestamp.toString(Qt::ISODate);
    
    QJsonDocument doc(jsonMessage);
    m_webSocket->sendTextMessage(QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
    
    qDebug() << "Message sent:" << message.content;
}

void NetworkClient::sendImage(const QString &serverId, const QByteArray &imageData)
{
    if (!m_isConnected) {
        qWarning() << "Not connected to server";
        return;
    }
    
    // TODO: Implement image sending with proper encoding and validation
    qDebug() << "Sending image to server:" << serverId;
}

void NetworkClient::sendLink(const QString &serverId, const QString &url)
{
    if (!m_isConnected) {
        qWarning() << "Not connected to server";
        return;
    }
    
    // TODO: Validate link before sending
    qDebug() << "Sending link to server:" << serverId << url;
}

bool NetworkClient::isConnected() const
{
    return m_isConnected;
}

void NetworkClient::setupWebSocket()
{
    connect(m_webSocket.get(), &QWebSocket::connected, this, &NetworkClient::onConnected);
    connect(m_webSocket.get(), &QWebSocket::disconnected, this, &NetworkClient::onDisconnected);
    connect(m_webSocket.get(), QOverload<const QString &>::of(&QWebSocket::textMessageReceived),
            this, &NetworkClient::onTextMessageReceived);
    connect(m_webSocket.get(), QOverload<const QByteArray &>::of(&QWebSocket::binaryMessageReceived),
            this, &NetworkClient::onBinaryMessageReceived);
    connect(m_webSocket.get(), QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
            this, &NetworkClient::onError);
}

void NetworkClient::onConnected()
{
    m_isConnected = true;
    m_reconnectAttempts = 0;
    
    qDebug() << "Connected to WebSocket server";
    
    // Send any queued messages
    while (!m_messageQueue.isEmpty()) {
        sendMessage(m_messageQueue.dequeue());
    }
}

void NetworkClient::onDisconnected()
{
    m_isConnected = false;
    qDebug() << "Disconnected from WebSocket server";
    reconnect();
}

void NetworkClient::onTextMessageReceived(const QString &message)
{
    qDebug() << "Message received:" << message;
    parseMessage(message);
}

void NetworkClient::onBinaryMessageReceived(const QByteArray &data)
{
    qDebug() << "Binary message received, size:" << data.size();
    
    // TODO: Handle image and link data
}

void NetworkClient::onError(QAbstractSocket::SocketError error)
{
    qWarning() << "WebSocket error:" << error << m_webSocket->errorString();
    emit connectionError(m_webSocket->errorString());
}

void NetworkClient::onSslErrors(const QList<QSslError> &errors)
{
    qWarning() << "SSL Errors:";
    for (const auto &error : errors) {
        qWarning() << error.errorString();
    }
    
    // In development, ignore SSL errors (NOT for production)
    // m_webSocket->ignoreSslErrors();
}

void NetworkClient::parseMessage(const QString &data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
    if (!doc.isObject()) {
        qWarning() << "Invalid JSON received";
        return;
    }
    
    QJsonObject obj = doc.object();
    QString messageType = obj["type"].toString();
    
    if (messageType == "message") {
        Message msg;
        msg.id = obj["id"].toString();
        msg.sender = obj["sender"].toString();
        msg.content = obj["content"].toString();
        msg.serverId = obj["serverId"].toString();
        msg.timestamp = QDateTime::fromString(obj["timestamp"].toString(), Qt::ISODate);
        
        emit messageReceived(msg);
    } else if (messageType == "linkValidation") {
        QString url = obj["url"].toString();
        bool isMalicious = obj["isMalicious"].toBool();
        emit linkValidationResult(url, isMalicious);
    }
}

void NetworkClient::serializeMessage(const Message &message)
{
    // Implementation handled in sendMessage()
}

void NetworkClient::reconnect()
{
    if (m_reconnectAttempts < m_maxReconnectAttempts) {
        m_reconnectAttempts++;
        
        int delayMs = m_config.reconnectDelayMs * m_reconnectAttempts;
        qDebug() << "Reconnecting in" << delayMs << "ms (attempt" << m_reconnectAttempts << ")";
        
        QTimer::singleShot(delayMs, this, [this]() {
            connectToServer(m_config.serverAddress, m_config.port);
        });
    } else {
        qWarning() << "Max reconnect attempts reached";
    }
}
