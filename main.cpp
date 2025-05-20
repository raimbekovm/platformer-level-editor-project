#include <QApplication>
#include <QTimer>
#include "main_window.h"
#include "splash_screen.h"
#include "welcome_screen.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setStyle("Fusion");

    QPixmap splashPixmap("data/splash.png");
    if (splashPixmap.isNull()) {
        splashPixmap = QPixmap(400, 300);
        splashPixmap.fill(QColor(33, 150, 243));
    }
    SplashScreen splash(splashPixmap);
    splash.show();

    splash.setMessage("Loading resources...");
    splash.setProgress(20);
    QTimer::singleShot(500, [&splash]() {
        splash.setMessage("Initializing editor...");
        splash.setProgress(50);
    });
    QTimer::singleShot(1000, [&splash]() {
        splash.setMessage("Almost ready...");
        splash.setProgress(80);
    });

    MainWindow mainWindow;
    mainWindow.hide();

    WelcomeScreen welcomeScreen;
    welcomeScreen.hide();

    QObject::connect(&welcomeScreen, &WelcomeScreen::newLevelRequested, [&]() {
        welcomeScreen.hide();
        mainWindow.show();
        mainWindow.newLevel();
    });

    QObject::connect(&welcomeScreen, &WelcomeScreen::openLevelRequested, [&]() {
        welcomeScreen.hide();
        mainWindow.show();
        mainWindow.openLevel();
    });

    QObject::connect(&welcomeScreen, &WelcomeScreen::recentLevelRequested, [&](const QString& path) {
        welcomeScreen.hide();
        mainWindow.show();
        mainWindow.openLevel(path);
    });

    QTimer::singleShot(1500, [&]() {
        splash.setMessage("Ready!");
        splash.setProgress(100);
        QTimer::singleShot(500, [&]() {
            splash.finish(&welcomeScreen);
            welcomeScreen.show();
        });
    });

    return app.exec();
}
