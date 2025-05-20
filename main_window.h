#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QMap>
#include <QPixmap>
#include <QStack>
#include <QPushButton>
#include <QDialog>
#include <QFileDialog>
#include <QFile>
#include <QMouseEvent>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    struct TileAction
    {
        int row;
        int col;
        QString previousType;
        QString newType;
    };

    enum class TileType
    {
        Coin     = '*',
        Enemy    = '&',
        Exit     = 'E',
        Player   = '@',
        Spikes   = '^',
        Wall     = '#',
        DarkWall = '=',
        Air      = ' '
    };

    void selectTile(char tile);
    void onTileClicked(int row, int col);
    void undoTilePlacement();

    void clearLevel();
    void resizeLevel();

    void exportToFile();

    TileType selectedTile;
    QStack<TileAction> undoStack;

    QTableWidget* gridWidget;
    QMap<QString, QPixmap> tileSprites;
    
    void setupGrid();
    void loadSprites();
    void setupGridCell(int row, int col);

    QPushButton* createButton(const QIcon &icon, std::function<void()> action);
};

#endif // MAIN_WINDOW_H
