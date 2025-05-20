#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QPixmap>
#include <QLabel>
#include <QStatusBar>
#include "grid_widget.h"
#include "tile_toolbar.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onTileSelected(char type);
    void onTilePlaced(int row, int col, const QString& type);
    void onClearGrid();
    void onResizeGrid();
    void onUndo();

private:
    void setupUI();
    void loadTileSprites();
    void setupEditToolbar();
    void setupMenuBar();
    void setupStatusBar();
    void updateStatusBar(int row, int col, const QString& type);

    GridWidget* gridWidget;
    TileToolbar* tileToolbar;
    QToolBar* editToolBar;
    QMap<QString, QPixmap> tileSprites;
    QLabel* statusLabel;
};

#endif // MAIN_WINDOW_H
