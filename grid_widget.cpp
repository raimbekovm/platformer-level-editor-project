#include "grid_widget.h"
#include <QHeaderView>
#include <QMouseEvent>
#include <QTableWidgetItem>
#include <QKeyEvent>

// Initialize grid with default size and setup
GridWidget::GridWidget(QWidget *parent)
    : QTableWidget(parent)
    , currentTileType(' ')
    , isDragging(false)
    , isSelecting(false)
    , isMovingSelection(false)
{
    setupGrid();
}

GridWidget::~GridWidget() {
}

// Setup grid with default size and cell properties
void GridWidget::setupGrid() {
    const int rows = 20;
    const int cols = 20;
    setRowCount(rows);
    setColumnCount(cols);

    const int cellSize = 32;
    for (int i = 0; i < rows; ++i) {
        setRowHeight(i, cellSize);
        for (int j = 0; j < cols; ++j) {
            setColumnWidth(j, cellSize);
            setupGridCell(i, j);
        }
    }

    horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    horizontalHeader()->hide();
    verticalHeader()->hide();

    setSelectionMode(QTableWidget::SingleSelection);
    setEditTriggers(QTableWidget::NoEditTriggers);
    setShowGrid(true);
    setGridStyle(Qt::SolidLine);
    
    setStyleSheet(
        "QTableWidget {"
        "   gridline-color: #808080;"
        "   background-color: white;"
        "}"
        "QTableWidget::item {"
        "   padding: 2px;"
        "}"
        "QTableWidget::item:selected {"
        "   background-color: #e0e0e0;"
        "}"
    );
}

// Initialize individual grid cell
void GridWidget::setupGridCell(int row, int col) {
    QTableWidgetItem* item = new QTableWidgetItem();
    item->setTextAlignment(Qt::AlignCenter);
    setItem(row, col, item);
}

// Clear all tiles and reset state
void GridWidget::clearGrid() {
    for (int row = 0; row < rowCount(); ++row) {
        for (int col = 0; col < columnCount(); ++col) {
            QTableWidgetItem* item = this->item(row, col);
            if (item) {
                item->setIcon(QIcon());
                item->setText("");
            }
        }
    }
    undo.clear();
    selection.clearSelection();
}

// Resize grid and maintain cell properties
void GridWidget::resizeGrid(int newRows, int newCols) {
    setRowCount(newRows);
    setColumnCount(newCols);

    const int cellSize = 32;
    for (int i = 0; i < newRows; ++i) {
        setRowHeight(i, cellSize);
        for (int j = 0; j < newCols; ++j) {
            setColumnWidth(j, cellSize);
            setupGridCell(i, j);
        }
    }
}

// Handle undo operation
bool GridWidget::undoLastAction() {
    if (undo.isEmpty()) return false;
    const auto& action = undo.getLastAction();
    applyTileAction(action);
    return undo.undoLastAction();
}

// Set available tile sprites
void GridWidget::setTileSprites(const QMap<QString, QPixmap>& sprites) {
    tileSprites = sprites;
}

// Get sprite path for tile type
QString GridWidget::getTileIconPath(char type) const {
    switch (type) {
        case '#': return "wall.png";
        case '=': return "wall_dark.png";
        case '@': return "player.png";
        case '*': return "coin.png";
        case '^': return "spikes.png";
        case '&': return "enemy.png";
        case 'E': return "exit.png";
        default: return "";
    }
}

// Place tile at specified position
void GridWidget::placeTile(int row, int col, char type) {
    QTableWidgetItem* item = this->item(row, col);
    if (!item) return;

    QString iconPath = getTileIconPath(type);
    if (!iconPath.isEmpty() && tileSprites.contains(iconPath)) {
        item->setIcon(QIcon(tileSprites[iconPath]));
        item->setText(QString(type));
    } else {
        item->setIcon(QIcon());
        item->setText(QString(type));
    }
}

// Handle tile placement during mouse events
void GridWidget::handleTilePlacement(const QPoint& pos) {
    QTableWidgetItem* item = itemAt(pos);
    if (item) {
        int row = item->row();
        int col = item->column();
        
        QString previousType = item->text();
        QString newType = QString(currentTileType);
        
        placeTile(row, col, currentTileType);
        undo.addAction(row, col, previousType, newType);
        emit tilePlaced(row, col, newType);
    }
}

// Apply undo action to grid
void GridWidget::applyTileAction(const GridUndo::TileAction& action) {
    QTableWidgetItem* item = this->item(action.row, action.col);
    if (item) {
        placeTile(action.row, action.col, action.previousType[0].toLatin1());
    }
}

// Get current state of all tiles
QMap<QPair<int, int>, QString> GridWidget::getGridState() const {
    QMap<QPair<int, int>, QString> state;
    for (int row = 0; row < rowCount(); ++row) {
        for (int col = 0; col < columnCount(); ++col) {
            QTableWidgetItem* item = this->item(row, col);
            if (item) {
                state[qMakePair(row, col)] = item->text();
            }
        }
    }
    return state;
}

// Handle mouse press events for tile placement and selection
void GridWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        QTableWidgetItem* item = itemAt(event->pos());
        if (!item) return;

        if (event->modifiers() & Qt::ShiftModifier) {
            // Start selection
            isSelecting = true;
            selectionStart = event->pos();
            selection.startSelection(item->row(), item->column(), item->text());
            
            // Visual feedback for selection
            item->setBackground(QColor(200, 200, 255, 100));
        } else if (!selection.isEmpty()) {
            // Start moving selection
            if (selection.getCurrentSelection().tiles.contains(qMakePair(item->row(), item->column()))) {
                isMovingSelection = true;
                moveStartPos = event->pos();
            } else {
                selection.clearSelection();
                isDragging = true;
                lastDragPos = event->pos();
                handleTilePlacement(event->pos());
            }
        } else {
            isDragging = true;
            lastDragPos = event->pos();
            handleTilePlacement(event->pos());
        }
    }
    QTableWidget::mousePressEvent(event);
}

// Handle mouse move events for dragging and selection
void GridWidget::mouseMoveEvent(QMouseEvent* event) {
    if (isSelecting) {
        QTableWidgetItem* startItem = itemAt(selectionStart);
        QTableWidgetItem* currentItem = itemAt(event->pos());
        if (startItem && currentItem) {
            // Clear previous selection visual feedback
            for (const auto& pos : selection.getCurrentSelection().tiles) {
                QTableWidgetItem* item = this->item(pos.first, pos.second);
                if (item) {
                    item->setBackground(Qt::transparent);
                }
            }

            // Update selection
            selection.updateSelection(startItem->row(), startItem->column(),
                                   currentItem->row(), currentItem->column(),
                                   getGridState());

            // Apply new selection visual feedback
            for (const auto& pos : selection.getCurrentSelection().tiles) {
                QTableWidgetItem* item = this->item(pos.first, pos.second);
                if (item) {
                    item->setBackground(QColor(200, 200, 255, 100));
                }
            }
        }
    } else if (isMovingSelection) {
        QPoint delta = event->pos() - moveStartPos;
        int dx = delta.x() / 32;  // cellSize
        int dy = delta.y() / 32;
        if (dx != 0 || dy != 0) {
            if (selection.isValidMove(dx, dy, rowCount(), columnCount())) {
                // Clear old positions
                for (const auto& pos : selection.getCurrentSelection().tiles) {
                    QTableWidgetItem* item = this->item(pos.first, pos.second);
                    if (item) {
                        item->setIcon(QIcon());
                        item->setText("");
                        item->setBackground(Qt::transparent);
                    }
                }

                // Move selection
                selection.moveSelection(dx, dy, rowCount(), columnCount());

                // Update new positions
                for (const auto& pos : selection.getCurrentSelection().tiles) {
                    placeTile(pos.first, pos.second, selection.getCurrentSelection().tileTypes[pos][0].toLatin1());
                    QTableWidgetItem* item = this->item(pos.first, pos.second);
                    if (item) {
                        item->setBackground(QColor(200, 200, 255, 100));
                    }
                }

                moveStartPos = event->pos();
            }
        }
    } else if (isDragging && (event->pos() - lastDragPos).manhattanLength() > 5) {
        handleTilePlacement(event->pos());
        lastDragPos = event->pos();
    }
    QTableWidget::mouseMoveEvent(event);
}

// Handle mouse release events
void GridWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        isDragging = false;
        isSelecting = false;
        isMovingSelection = false;
    }
    QTableWidget::mouseReleaseEvent(event);
}

// Handle keyboard events for selection movement
void GridWidget::keyPressEvent(QKeyEvent* event) {
    if (!selection.isEmpty()) {
        int dx = 0, dy = 0;
        switch (event->key()) {
            case Qt::Key_Left: dx = -1; break;
            case Qt::Key_Right: dx = 1; break;
            case Qt::Key_Up: dy = -1; break;
            case Qt::Key_Down: dy = 1; break;
            case Qt::Key_Escape: selection.clearSelection(); break;
            default: QTableWidget::keyPressEvent(event); return;
        }
        if ((dx != 0 || dy != 0) && selection.isValidMove(dx, dy, rowCount(), columnCount())) {
            selection.moveSelection(dx, dy, rowCount(), columnCount());
        }
    } else {
        QTableWidget::keyPressEvent(event);
    }
} 