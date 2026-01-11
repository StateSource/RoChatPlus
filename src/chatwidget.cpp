#include "chatwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDateTime>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include "include/constants.h"

ChatWidget::ChatWidget(const Server &server, QWidget *parent)
    : QWidget(parent)
    , m_server(server)
    , m_splitter(new QSplitter(Qt::Horizontal, this))
    , m_chatDisplay(new QTextEdit(this))
    , m_messageInput(new QLineEdit(this))
    , m_sendButton(new QPushButton(tr("Send"), this))
    , m_imageButton(new QPushButton(tr("📷 Image"), this))
    , m_linkButton(new QPushButton(tr("🔗 Link"), this))
    , m_userList(new QListWidget(this))
{
    setupUI();
    connectSignals();
}

ChatWidget::~ChatWidget() = default;

void ChatWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Splitter for chat display and user list
    m_chatDisplay->setReadOnly(true);
    m_chatDisplay->setStyleSheet("QTextEdit { background-color: #2b2b2b; color: #ffffff; }");
    
    m_userList->setMaximumWidth(200);
    m_userList->setStyleSheet("QListWidget { background-color: #3b3b3b; color: #ffffff; }");
    
    m_splitter->addWidget(m_chatDisplay);
    m_splitter->addWidget(m_userList);
    m_splitter->setStretchFactor(0, 1);
    m_splitter->setStretchFactor(1, 0);
    
    mainLayout->addWidget(m_splitter);
    
    // Input area
    QHBoxLayout *inputLayout = new QHBoxLayout();
    
    m_messageInput->setPlaceholderText(tr("Type a message..."));
    m_messageInput->setStyleSheet("QLineEdit { background-color: #3b3b3b; color: #ffffff; border: 1px solid #555; padding: 5px; }");
    
    m_sendButton->setMaximumWidth(80);
    m_imageButton->setMaximumWidth(80);
    m_linkButton->setMaximumWidth(80);
    
    inputLayout->addWidget(m_messageInput);
    inputLayout->addWidget(m_imageButton);
    inputLayout->addWidget(m_linkButton);
    inputLayout->addWidget(m_sendButton);
    
    mainLayout->addLayout(inputLayout);
    
    setLayout(mainLayout);
    
    qDebug() << "ChatWidget UI setup complete for server:" << m_server.name;
}

void ChatWidget::connectSignals()
{
    connect(m_sendButton, &QPushButton::clicked, this, &ChatWidget::onSendButtonClicked);
    connect(m_messageInput, &QLineEdit::returnPressed, this, &ChatWidget::onMessageInputReturnPressed);
    connect(m_imageButton, &QPushButton::clicked, this, &ChatWidget::onImageButtonClicked);
    connect(m_linkButton, &QPushButton::clicked, this, &ChatWidget::onLinkButtonClicked);
}

void ChatWidget::displayMessage(const Message &message)
{
    formatMessageDisplay(message);
}

void ChatWidget::setServer(const Server &server)
{
    m_server = server;
    m_chatDisplay->clear();
    m_userList->clear();
}

void ChatWidget::onSendButtonClicked()
{
    QString messageText = m_messageInput->text().trimmed();
    
    if (messageText.isEmpty()) {
        return;
    }
    
    if (messageText.length() > Constants::MAX_MESSAGE_LENGTH) {
        QMessageBox::warning(this, tr("Message Too Long"),
            tr("Message exceeds maximum length of %1 characters").arg(Constants::MAX_MESSAGE_LENGTH));
        return;
    }
    
    Message message;
    message.sender = "CurrentUser";  // TODO: Get actual username
    message.content = messageText;
    message.serverId = m_server.id;
    message.timestamp = QDateTime::currentDateTime();
    
    emit messageSent(message);
    
    formatMessageDisplay(message);
    m_messageInput->clear();
    m_messageInput->setFocus();
}

void ChatWidget::onMessageInputReturnPressed()
{
    onSendButtonClicked();
}

void ChatWidget::onImageButtonClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select Image"),
        "", tr("Images (*.png *.jpg *.jpeg *.bmp);;All Files (*)"));
    
    if (!fileName.isEmpty()) {
        qDebug() << "Image selected:" << fileName;
        // TODO: Handle image upload and encoding
    }
}

void ChatWidget::onLinkButtonClicked()
{
    // TODO: Show dialog to add link
    qDebug() << "Link button clicked";
}

void ChatWidget::formatMessageDisplay(const Message &message)
{
    QString formattedMessage = QString("<b>%1</b> <span style='color: #888;'>%2</span><br/>%3<br/>")
        .arg(message.sender)
        .arg(message.timestamp.toString("hh:mm:ss"))
        .arg(message.content.replace("<", "&lt;").replace(">", "&gt;"));
    
    appendMessageToDisplay(formattedMessage);
}

void ChatWidget::appendMessageToDisplay(const QString &formattedMessage)
{
    m_chatDisplay->append(formattedMessage);
    
    // Scroll to bottom
    QTextCursor cursor = m_chatDisplay->textCursor();
    cursor.movePosition(QTextCursor::End);
    m_chatDisplay->setTextCursor(cursor);
}
