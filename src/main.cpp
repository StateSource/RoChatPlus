#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include <QDebug>
#include "mainwindow.h"
#include "include/constants.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Set application information
    app.setApplicationName(Constants::APP_NAME);
    app.setApplicationVersion(Constants::APP_VERSION);
    app.setApplicationAuthor(Constants::APP_AUTHOR);

    // Enable high DPI scaling for Windows
    app.setAttribute(Qt::AA_EnableHighDpiScaling);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);

    qDebug() << "Starting" << Constants::APP_NAME << "v" << Constants::APP_VERSION;

    // Create and show main window
    MainWindow window;
    window.show();

    return app.exec();
}
