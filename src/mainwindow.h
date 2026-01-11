#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QMap>
#include <memory>
#include "chatwidget.h"
#include "networkclient.h"
#include "include/types.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onAddServerTab();
    void onRemoveServerTab(int index);
    void onTabChanged(int index);
    void onServerConnected(const QString &serverId);
    void onServerDisconnected(const QString &serverId);
    void onMessageReceived(const Message &message);
    void onShowSettings();
    void onShowAbout();
    void onSystemTrayActivated(QSystemTrayIcon::ActivationReason reason);

private:
    void setupUI();
    void setupMenuBar();
    void setupSystemTray();
    void createChatTab(const Server &server);
    void connectSignals();
    void loadServers();
    void saveServers();
    void restoreWindowState();
    void saveWindowState();

    QTabWidget *m_tabWidget;
    QMap<QString, std::unique_ptr<ChatWidget>> m_chatWidgets;
    QMap<QString, Server> m_servers;
    std::unique_ptr<NetworkClient> m_networkClient;
    QSystemTrayIcon *m_trayIcon;
    
    ChatConfig m_chatConfig;
    NetworkConfig m_networkConfig;
};

#endif // MAINWINDOW_H
