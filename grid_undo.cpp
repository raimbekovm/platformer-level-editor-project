#include "grid_undo.h"

GridUndo::GridUndo() {
}

void GridUndo::addAction(int row, int col, const QString& previousType, const QString& newType) {
    TileAction action{row, col, previousType, newType};
    undoStack.push(action);
}

bool GridUndo::undoLastAction() {
    if (undoStack.isEmpty()) return false;
    undoStack.pop();
    return true;
}

const GridUndo::TileAction& GridUndo::getLastAction() const {
    return undoStack.top();
} 