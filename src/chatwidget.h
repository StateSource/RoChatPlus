#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QSplitter>
#include "include/types.h"

class ChatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWidget(const Server &server, QWidget *parent = nullptr);
    ~ChatWidget() override;

    void displayMessage(const Message &message);
    void setServer(const Server &server);
    const Server &getServer() const { return m_server; }

signals:
    void messageSent(const Message &message);
    void connectionStatusChanged(bool connected);

private slots:
    void onSendButtonClicked();
    void onMessageInputReturnPressed();
    void onImageButtonClicked();
    void onLinkButtonClicked();

private:
    void setupUI();
    void connectSignals();
    void formatMessageDisplay(const Message &message);
    void appendMessageToDisplay(const QString &formattedMessage);

    Server m_server;
    
    // UI Components
    QSplitter *m_splitter;
    QTextEdit *m_chatDisplay;
    QLineEdit *m_messageInput;
    QPushButton *m_sendButton;
    QPushButton *m_imageButton;
    QPushButton *m_linkButton;
    QListWidget *m_userList;
};

#endif // CHATWIDGET_H
