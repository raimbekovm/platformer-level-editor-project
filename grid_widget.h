#ifndef GRID_WIDGET_H
#define GRID_WIDGET_H

#include <QTableWidget>
#include <QMap>
#include <QPixmap>
#include "grid_selection.h"
#include "grid_undo.h"

// Grid-based level editor widget with tile placement and selection support
class GridWidget : public QTableWidget
{
    Q_OBJECT

public:
    explicit GridWidget(QWidget *parent = nullptr);
    ~GridWidget();

    // Core grid operations
    void clearGrid();
    void resizeGrid(int newRows, int newCols);
    bool undoLastAction();
    void setTileSprites(const QMap<QString, QPixmap>& sprites);
    void placeTile(int row, int col, char type);
    void setCurrentTileType(char type) { currentTileType = type; }

signals:
    void tilePlaced(int row, int col, const QString& type);

protected:
    // Mouse and keyboard event handlers
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    // Grid setup and tile management
    void setupGrid();
    void setupGridCell(int row, int col);
    QString getTileIconPath(char type) const;
    void handleTilePlacement(const QPoint& pos);
    void applyTileAction(const GridUndo::TileAction& action);
    QMap<QPair<int, int>, QString> getGridState() const;

    // Member variables
    QMap<QString, QPixmap> tileSprites;
    char currentTileType;
    bool isDragging;
    QPoint lastDragPos;
    bool isSelecting;
    QPoint selectionStart;
    bool isMovingSelection;
    QPoint moveStartPos;
    GridSelection selection;
    GridUndo undo;
};

#endif // GRID_WIDGET_H 