#ifndef WELCOME_SCREEN_H
#define WELCOME_SCREEN_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>

class WelcomeScreen : public QDialog
{
    Q_OBJECT

public:
    explicit WelcomeScreen(QWidget *parent = nullptr);

signals:
    void newLevelRequested();
    void openLevelRequested();
    void recentLevelRequested(const QString& path);

private:
    void setupUI();
    void createRecentLevelsList();
    void updateRecentLevels();

    QPushButton* newLevelButton;
    QPushButton* openLevelButton;
    QLabel* recentLevelsLabel;
};

#endif // WELCOME_SCREEN_H 