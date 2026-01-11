#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QSystemTrayIcon>
#include <QSettings>
#include <QMessageBox>
#include <QDebug>
#include "include/constants.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_tabWidget(new QTabWidget(this))
    , m_networkClient(std::make_unique<NetworkClient>(this))
    , m_trayIcon(new QSystemTrayIcon(this))
{
    setWindowTitle(QString("%1 v%2").arg(Constants::APP_NAME, Constants::APP_VERSION));
    setWindowIcon(QIcon(":/icons/app_icon.png"));
    
    // Set default window size
    setGeometry(100, 100, 1000, 600);

    setupUI();
    setupMenuBar();
    setupSystemTray();
    connectSignals();
    
    restoreWindowState();
    loadServers();

    qDebug() << "MainWindow initialized";
}

MainWindow::~MainWindow()
{
    saveWindowState();
    saveServers();
    if (m_networkClient) {
        m_networkClient->disconnect();
    }
}

void MainWindow::setupUI()
{
    // Central widget
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    
    // Tab widget for multiple servers
    layout->addWidget(m_tabWidget);
    
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}

void MainWindow::setupMenuBar()
{
    // File menu
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    
    QAction *addServerAction = fileMenu->addAction(tr("&Add Server"));
    connect(addServerAction, &QAction::triggered, this, &MainWindow::onAddServerTab);
    
    fileMenu->addSeparator();
    
    QAction *exitAction = fileMenu->addAction(tr("E&xit"));
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    
    // Edit menu
    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    
    QAction *settingsAction = editMenu->addAction(tr("&Settings"));
    connect(settingsAction, &QAction::triggered, this, &MainWindow::onShowSettings);
    
    // Help menu
    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    
    QAction *aboutAction = helpMenu->addAction(tr("&About"));
    connect(aboutAction, &QAction::triggered, this, &MainWindow::onShowAbout);
}

void MainWindow::setupSystemTray()
{
    QMenu *trayMenu = new QMenu(this);
    
    QAction *showAction = trayMenu->addAction(tr("&Show"));
    connect(showAction, &QAction::triggered, [this]() { 
        setVisible(!isVisible());
        if (isVisible()) {
            activateWindow();
        }
    });
    
    trayMenu->addSeparator();
    
    QAction *quitAction = trayMenu->addAction(tr("&Quit"));
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);
    
    m_trayIcon->setContextMenu(trayMenu);
    m_trayIcon->setIcon(QIcon(":/icons/app_icon.png"));
    connect(m_trayIcon, QOverload<QSystemTrayIcon::ActivationReason>::of(&QSystemTrayIcon::activated),
            this, &MainWindow::onSystemTrayActivated);
    
    m_trayIcon->show();
}

void MainWindow::connectSignals()
{
    connect(m_tabWidget, QOverload<int>::of(&QTabWidget::currentChanged),
            this, &MainWindow::onTabChanged);
    
    connect(m_networkClient.get(), &NetworkClient::connected,
            this, &MainWindow::onServerConnected);
    
    connect(m_networkClient.get(), &NetworkClient::disconnected,
            this, &MainWindow::onServerDisconnected);
    
    connect(m_networkClient.get(), &NetworkClient::messageReceived,
            this, &MainWindow::onMessageReceived);
}

void MainWindow::createChatTab(const Server &server)
{
    auto chatWidget = std::make_unique<ChatWidget>(server, this);
    int index = m_tabWidget->addTab(chatWidget.get(), server.name);
    m_chatWidgets[server.id] = std::move(chatWidget);
    m_tabWidget->setCurrentIndex(index);
}

void MainWindow::onAddServerTab()
{
    // TODO: Show dialog to add new server
    qDebug() << "Add server tab requested";
}

void MainWindow::onRemoveServerTab(int index)
{
    QString serverId;
    for (auto it = m_chatWidgets.begin(); it != m_chatWidgets.end(); ++it) {
        if (m_tabWidget->indexOf(it.value().get()) == index) {
            serverId = it.key();
            break;
        }
    }
    
    if (!serverId.isEmpty()) {
        m_chatWidgets.remove(serverId);
        m_servers.remove(serverId);
        m_tabWidget->removeTab(index);
    }
}

void MainWindow::onTabChanged(int index)
{
    qDebug() << "Tab changed to index:" << index;
}

void MainWindow::onServerConnected(const QString &serverId)
{
    qDebug() << "Connected to server:" << serverId;
    if (m_servers.contains(serverId)) {
        m_servers[serverId].isConnected = true;
    }
}

void MainWindow::onServerDisconnected(const QString &serverId)
{
    qDebug() << "Disconnected from server:" << serverId;
    if (m_servers.contains(serverId)) {
        m_servers[serverId].isConnected = false;
    }
}

void MainWindow::onMessageReceived(const Message &message)
{
    if (m_chatWidgets.contains(message.serverId)) {
        m_chatWidgets[message.serverId]->displayMessage(message);
    }
}

void MainWindow::onShowSettings()
{
    // TODO: Implement settings dialog
    qDebug() << "Show settings requested";
}

void MainWindow::onShowAbout()
{
    QMessageBox::about(this, tr("About %1").arg(Constants::APP_NAME),
        tr("%1 v%2\n\nA modern chat client for Roblox\n\nAuthor: %3")
            .arg(Constants::APP_NAME, Constants::APP_VERSION, Constants::APP_AUTHOR));
}

void MainWindow::onSystemTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::DoubleClick) {
        setVisible(!isVisible());
        if (isVisible()) {
            activateWindow();
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (m_trayIcon->isVisible()) {
        hide();
        event->ignore();
    } else {
        event->accept();
    }
}

void MainWindow::loadServers()
{
    QSettings settings(Constants::CONFIG_PATH, QSettings::IniFormat);
    // TODO: Load server list from settings
}

void MainWindow::saveServers()
{
    QSettings settings(Constants::CONFIG_PATH, QSettings::IniFormat);
    // TODO: Save server list to settings
}

void MainWindow::restoreWindowState()
{
    QSettings settings(Constants::CONFIG_PATH, QSettings::IniFormat);
    restoreGeometry(settings.value("mainWindow/geometry", saveGeometry()).toByteArray());
    restoreState(settings.value("mainWindow/state", saveState()).toByteArray());
}

void MainWindow::saveWindowState()
{
    QSettings settings(Constants::CONFIG_PATH, QSettings::IniFormat);
    settings.setValue("mainWindow/geometry", saveGeometry());
    settings.setValue("mainWindow/state", saveState());
}
