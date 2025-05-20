#ifndef SPLASH_SCREEN_H
#define SPLASH_SCREEN_H

#include <QSplashScreen>
#include <QProgressBar>
#include <QLabel>

class SplashScreen : public QSplashScreen
{
    Q_OBJECT

public:
    explicit SplashScreen(const QPixmap& pixmap = QPixmap());
    void setProgress(int value);
    void setMessage(const QString& message);

protected:
    void drawContents(QPainter* painter) override;

private:
    QProgressBar* progressBar;
    QLabel* messageLabel;
    void setupUI();
};

#endif // SPLASH_SCREEN_H 