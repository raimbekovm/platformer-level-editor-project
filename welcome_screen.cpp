#include "welcome_screen.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>

WelcomeScreen::WelcomeScreen(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
    createRecentLevelsList();
    updateRecentLevels();
}

void WelcomeScreen::setupUI() {
    setWindowTitle("Welcome to Level Editor");
    setFixedSize(600, 400);
    setStyleSheet(
        "QDialog {"
        "   background-color: #f0f0f0;"
        "}"
        "QLabel {"
        "   color: #333333;"
        "   font-size: 14px;"
        "}"
        "QPushButton {"
        "   background-color: #2196F3;"
        "   color: white;"
        "   border: none;"
        "   padding: 10px 20px;"
        "   border-radius: 5px;"
        "   font-size: 14px;"
        "   min-width: 120px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #1976D2;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #0D47A1;"
        "}"
        "QListWidget {"
        "   background-color: white;"
        "   border: 1px solid #cccccc;"
        "   border-radius: 5px;"
        "   padding: 5px;"
        "}"
        "QListWidget::item {"
        "   padding: 5px;"
        "   border-bottom: 1px solid #eeeeee;"
        "}"
        "QListWidget::item:hover {"
        "   background-color: #e3f2fd;"
        "}"
    );

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // Title
    QLabel* titleLabel = new QLabel("Welcome to Level Editor", this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #1976D2;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(20);

    newLevelButton = new QPushButton("New Level", this);
    openLevelButton = new QPushButton("Open Level", this);

    buttonLayout->addWidget(newLevelButton);
    buttonLayout->addWidget(openLevelButton);
    mainLayout->addLayout(buttonLayout);

    // Recent Levels
    recentLevelsLabel = new QLabel("Recent Levels", this);
    mainLayout->addWidget(recentLevelsLabel);

    // Recent levels list will be added in createRecentLevelsList()
    mainLayout->addStretch();

    // Connect signals
    connect(newLevelButton, &QPushButton::clicked, this, &WelcomeScreen::newLevelRequested);
    connect(openLevelButton, &QPushButton::clicked, this, [this]() {
        QString fileName = QFileDialog::getOpenFileName(this,
            "Open Level", "", "Level Files (*.level);;All Files (*)");
        if (!fileName.isEmpty()) {
            emit openLevelRequested();
        }
    });
}

void WelcomeScreen::createRecentLevelsList() {
    QListWidget* recentList = new QListWidget(this);
    recentList->setStyleSheet(
        "QListWidget {"
        "   background-color: white;"
        "   border: 1px solid #cccccc;"
        "   border-radius: 5px;"
        "}"
    );

    // Add to layout
    layout()->addWidget(recentList);

    // Connect double-click signal
    connect(recentList, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem* item) {
        emit recentLevelRequested(item->data(Qt::UserRole).toString());
    });
}

void WelcomeScreen::updateRecentLevels() {
    QListWidget* recentList = findChild<QListWidget*>();
    if (!recentList) return;

    recentList->clear();

    QSettings settings;
    QStringList recentFiles = settings.value("recentFiles").toStringList();

    for (const QString& file : recentFiles) {
        QListWidgetItem* item = new QListWidgetItem(QFileInfo(file).fileName());
        item->setData(Qt::UserRole, file);
        recentList->addItem(item);
    }
} 