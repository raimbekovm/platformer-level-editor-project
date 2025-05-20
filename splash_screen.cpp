#include "splash_screen.h"
#include <QVBoxLayout>
#include <QPainter>
#include <QApplication>

SplashScreen::SplashScreen(const QPixmap& pixmap)
    : QSplashScreen(pixmap)
{
    setupUI();
}

void SplashScreen::setupUI() {
    // Create a widget to hold our controls
    QWidget* container = new QWidget(this);
    container->setStyleSheet(
        "QWidget {"
        "   background-color: rgba(0, 0, 0, 0);"
        "}"
    );

    // Create layout
    QVBoxLayout* layout = new QVBoxLayout(container);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(10);

    // Create message label
    messageLabel = new QLabel(this);
    messageLabel->setStyleSheet(
        "QLabel {"
        "   color: white;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   background-color: rgba(0, 0, 0, 0);"
        "}"
    );
    messageLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(messageLabel);

    // Create progress bar
    progressBar = new QProgressBar(this);
    progressBar->setStyleSheet(
        "QProgressBar {"
        "   border: 2px solid #2196F3;"
        "   border-radius: 5px;"
        "   text-align: center;"
        "   background-color: rgba(255, 255, 255, 0.2);"
        "}"
        "QProgressBar::chunk {"
        "   background-color: #2196F3;"
        "   border-radius: 3px;"
        "}"
    );
    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    progressBar->setFixedHeight(10);
    layout->addWidget(progressBar);

    // Position the container at the bottom of the splash screen
    container->setGeometry(0, height() - 100, width(), 80);
}

void SplashScreen::setProgress(int value) {
    progressBar->setValue(value);
    QApplication::processEvents();
}

void SplashScreen::setMessage(const QString& message) {
    messageLabel->setText(message);
    QApplication::processEvents();
}

void SplashScreen::drawContents(QPainter* painter) {
    QSplashScreen::drawContents(painter);
} 