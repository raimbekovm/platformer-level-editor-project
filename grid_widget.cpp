#include "grid_widget.h"
#include <QHeaderView>
#include <QMouseEvent>
#include <QTableWidgetItem>

GridWidget::GridWidget(QWidget *parent)
    : QTableWidget(parent)
    , currentTileType(' ')
{
    setupGrid();
}

GridWidget::~GridWidget() {
}

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

void GridWidget::setupGridCell(int row, int col) {
    QTableWidgetItem* item = new QTableWidgetItem();
    item->setTextAlignment(Qt::AlignCenter);
    setItem(row, col, item);
}

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
    undoStack.clear();
}

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

bool GridWidget::undoLastAction() {
    if (undoStack.isEmpty()) return false;
    TileAction action = undoStack.pop();
    applyTileAction(action);
    return true;
}

void GridWidget::addAction(int row, int col, const QString& previousType, const QString& newType) {
    TileAction action{row, col, previousType, newType};
    undoStack.push(action);
}

void GridWidget::applyTileAction(const TileAction& action) {
    QTableWidgetItem* item = this->item(action.row, action.col);
    if (item) {
        placeTile(action.row, action.col, action.previousType[0].toLatin1());
    }
}

void GridWidget::setTileSprites(const QMap<QString, QPixmap>& sprites) {
    tileSprites = sprites;
}

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

void GridWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        QTableWidgetItem* item = itemAt(event->pos());
        if (item) {
            int row = item->row();
            int col = item->column();
            
            QString previousType = item->text();
            QString newType = QString(currentTileType);
            
            placeTile(row, col, currentTileType);
            addAction(row, col, previousType, newType);
            emit tilePlaced(row, col, newType);
        }
    }
    QTableWidget::mousePressEvent(event);
} 