#ifndef GRID_UNDO_H
#define GRID_UNDO_H

#include <QStack>
#include <QString>

class GridUndo {
public:
    struct TileAction {
        int row;
        int col;
        QString previousType;
        QString newType;
    };

    GridUndo();
    void addAction(int row, int col, const QString& previousType, const QString& newType);
    bool undoLastAction();
    const TileAction& getLastAction() const;
    bool isEmpty() const { return undoStack.isEmpty(); }
    void clear() { undoStack.clear(); }

private:
    QStack<TileAction> undoStack;
};

#endif // GRID_UNDO_H 