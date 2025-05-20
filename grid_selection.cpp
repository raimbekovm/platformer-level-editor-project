#include "grid_selection.h"

GridSelection::GridSelection() {
}

void GridSelection::startSelection(int row, int col, const QString& type) {
    clearSelection();
    currentSelection.tiles.insert(qMakePair(row, col));
    currentSelection.tileTypes[qMakePair(row, col)] = type;
}

void GridSelection::updateSelection(int startRow, int startCol, int currentRow, int currentCol,
                                 const QMap<QPair<int, int>, QString>& gridState) {
    clearSelection();

    int rowMin = qMin(startRow, currentRow);
    int rowMax = qMax(startRow, currentRow);
    int colMin = qMin(startCol, currentCol);
    int colMax = qMax(startCol, currentCol);

    for (int row = rowMin; row <= rowMax; ++row) {
        for (int col = colMin; col <= colMax; ++col) {
            auto pos = qMakePair(row, col);
            if (gridState.contains(pos) && !gridState[pos].isEmpty()) {
                currentSelection.tiles.insert(pos);
                currentSelection.tileTypes[pos] = gridState[pos];
            }
        }
    }
}

void GridSelection::clearSelection() {
    currentSelection.tiles.clear();
    currentSelection.tileTypes.clear();
}

void GridSelection::moveSelection(int dx, int dy, int gridRows, int gridCols) {
    QSet<QPair<int, int>> newTiles;
    QMap<QPair<int, int>, QString> newTileTypes;

    for (const auto& pos : currentSelection.tiles) {
        int newRow = pos.first + dy;
        int newCol = pos.second + dx;
        if (newRow >= 0 && newRow < gridRows && newCol >= 0 && newCol < gridCols) {
            auto newPos = qMakePair(newRow, newCol);
            newTiles.insert(newPos);
            newTileTypes[newPos] = currentSelection.tileTypes[pos];
        }
    }

    currentSelection.tiles = newTiles;
    currentSelection.tileTypes = newTileTypes;
}

bool GridSelection::isValidMove(int dx, int dy, int gridRows, int gridCols) const {
    for (const auto& pos : currentSelection.tiles) {
        int newRow = pos.first + dy;
        int newCol = pos.second + dx;
        if (newRow < 0 || newRow >= gridRows || newCol < 0 || newCol >= gridCols) {
            return false;
        }
    }
    return true;
} 