#ifndef GRID_WIDGET_H
#define GRID_WIDGET_H

#include <QTableWidget>
#include <QStack>
#include <QMap>
#include <QPixmap>

class GridWidget : public QTableWidget
{
    Q_OBJECT

public:
    explicit GridWidget(QWidget *parent = nullptr);
    ~GridWidget();

    void clearGrid();
    void resizeGrid(int newRows, int newCols);
    bool undoLastAction();
    void addAction(int row, int col, const QString& previousType, const QString& newType);
    void setTileSprites(const QMap<QString, QPixmap>& sprites);
    void placeTile(int row, int col, char type);
    void setCurrentTileType(char type) { currentTileType = type; }

signals:
    void tilePlaced(int row, int col, const QString& type);

protected:
    void mousePressEvent(QMouseEvent* event) override;

private:
    struct TileAction
    {
        int row;
        int col;
        QString previousType;
        QString newType;
    };

    void setupGrid();
    void setupGridCell(int row, int col);
    void applyTileAction(const TileAction& action);
    QString getTileIconPath(char type) const;

    QStack<TileAction> undoStack;
    QMap<QString, QPixmap> tileSprites;
    char currentTileType;
};

#endif // GRID_WIDGET_H 