#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QPixmap>
#include <QLabel>
#include <QStatusBar>
#include <QMenuBar>
#include <QToolBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QTextEdit>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include "grid_widget.h"
#include "tile_toolbar.h"

class QToolBar;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void newLevel();
    void openLevel(const QString& path = QString());

private slots:
    void onTileSelected(char type);
    void onTilePlaced(int row, int col, const QString& type);
    void onClearGrid();
    void onResizeGrid();
    void onUndo();
    void exportLevel();
    void saveLevel();
    void showDocumentation();
    void showAbout();

private:
    void setupUI();
    void loadTileSprites();
    void setupEditToolbar();
    void setupMenuBar();
    void setupStatusBar();
    void updateStatusBar(int row, int col, const QString& type);
    QString getLevelData() const;
    void addToRecentFiles(const QString& filePath);
    void updateRecentFilesMenu();

    GridWidget* gridWidget;
    TileToolbar* tileToolbar;
    QToolBar* editToolBar;
    QMenu* recentFilesMenu;
    QString currentFilePath;
    QMap<QString, QPixmap> tileSprites;
    QLabel* statusLabel;
};

#endif // MAIN_WINDOW_H
