#ifndef GRID_SELECTION_H
#define GRID_SELECTION_H

#include <QSet>
#include <QMap>
#include <QPair>
#include <QString>

class GridSelection {
public:
    struct SelectionGroup {
        QSet<QPair<int, int>> tiles;
        QMap<QPair<int, int>, QString> tileTypes;
    };

    GridSelection();
    void startSelection(int row, int col, const QString& type);
    void updateSelection(int startRow, int startCol, int currentRow, int currentCol, 
                        const QMap<QPair<int, int>, QString>& gridState);
    void clearSelection();
    void moveSelection(int dx, int dy, int gridRows, int gridCols);
    bool isValidMove(int dx, int dy, int gridRows, int gridCols) const;
    const SelectionGroup& getCurrentSelection() const { return currentSelection; }
    bool isEmpty() const { return currentSelection.tiles.isEmpty(); }

private:
    SelectionGroup currentSelection;
};

#endif // GRID_SELECTION_H 