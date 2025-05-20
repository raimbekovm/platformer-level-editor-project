#include "main_window.h"
#include "utilities.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), selectedTile(TileType::Air)
{
    setWindowTitle("Platformer Level Editor");
    resize(800, 600);

    auto* centralWidget = new QWidget(this);
    auto* layout = new QVBoxLayout(centralWidget);
    setCentralWidget(centralWidget);

    gridWidget = new QTableWidget(this);
    setupGrid();
    layout->addWidget(gridWidget);

    loadSprites();
}

MainWindow::~MainWindow() {
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Z && event->modifiers() & Qt::ControlModifier) {
        undoTilePlacement();
    }
    QMainWindow::keyPressEvent(event);
}

void MainWindow::selectTile(char tile) {
    // TODO: Implement tile selection
}

void MainWindow::onTileClicked(int row, int col) {
    // TODO: Implement tile placement
}

void MainWindow::undoTilePlacement() {
    if (undoStack.isEmpty()) return;
    TileAction action = undoStack.pop();
    
    QTableWidgetItem* item = gridWidget->item(action.row, action.col);
    if (item) {
        item->setData(Qt::UserRole, action.previousType);
        if (action.previousType == "wall") {
            item->setIcon(tileSprites["wall.png"]);
        } else {
            item->setIcon(QIcon());
        }
    }
}

QPushButton* MainWindow::createButton(const QIcon &icon, std::function<void()> action) {
    QPushButton *button = new QPushButton();
    button->setIcon(icon);
    connect(button, &QPushButton::clicked, action);
    return button;
}

void MainWindow::clearLevel() {
    for (int row = 0; row < gridWidget->rowCount(); ++row) {
        for (int col = 0; col < gridWidget->columnCount(); ++col) {
            QTableWidgetItem* item = gridWidget->item(row, col);
            if (item) {
                item->setData(Qt::UserRole, "air");
                item->setIcon(QIcon());
            }
        }
    }
}

void MainWindow::resizeLevel() {
    QDialog resizeDialog(this);
    resizeDialog.setWindowTitle("Resize Level");
    
    // TODO: Add resize dialog implementation
    
    if (resizeDialog.exec() == QDialog::Accepted) {
        // TODO: Implement resize functionality
    }
}

void MainWindow::exportToFile() {
    QString filePath = QFileDialog::getSaveFileName(
        this,
        "Export Level",
        "",
        "RLL Files (*.rll);;All Files (*)"
    );

    if (filePath.isEmpty()) return;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Could not open file for writing");
        return;
    }

    QTextStream out(&file);
    // TODO: Implement export functionality
    file.close();
}

void MainWindow::setupGrid() {
    const int rows = 20;
    const int cols = 20;
    gridWidget->setRowCount(rows);
    gridWidget->setColumnCount(cols);

    const int cellSize = 32;
    for (int i = 0; i < rows; ++i) {
        gridWidget->setRowHeight(i, cellSize);
        for (int j = 0; j < cols; ++j) {
            gridWidget->setColumnWidth(j, cellSize);
            setupGridCell(i, j);
        }
    }

    // Make cells square and prevent resizing
    gridWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    gridWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    gridWidget->horizontalHeader()->hide();
    gridWidget->verticalHeader()->hide();

    // Enable cell selection
    gridWidget->setSelectionMode(QTableWidget::SingleSelection);
    gridWidget->setEditTriggers(QTableWidget::NoEditTriggers);
}

void MainWindow::loadSprites() {
    QString spritePath = "data/sprites/";
    QStringList spriteFiles = {
        "wall.png", "wall_dark.png", "coin.png", 
        "enemy.png", "exit.png", "player.png", "spikes.png"
    };

    for (const QString& file : spriteFiles) {
        QString fullPath = spritePath + file;
        QPixmap pixmap(fullPath);
        if (!pixmap.isNull()) {
            tileSprites[file] = pixmap.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        }
    }
}

void MainWindow::setupGridCell(int row, int col) {
    QTableWidgetItem* item = new QTableWidgetItem();
    item->setData(Qt::UserRole, "air");
    gridWidget->setItem(row, col, item);
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        QPoint pos = event->pos();
        QTableWidgetItem* item = gridWidget->itemAt(pos);
        if (item) {
            QString currentType = item->data(Qt::UserRole).toString();
            QString newType = (currentType == "air") ? "wall" : "air";
            
            TileAction action;
            action.row = item->row();
            action.col = item->column();
            action.previousType = currentType;
            action.newType = newType;
            undoStack.push(action);
            
            item->setData(Qt::UserRole, newType);
            if (newType == "wall") {
                item->setIcon(tileSprites["wall.png"]);
            } else {
                item->setIcon(QIcon());
            }
        }
    }
}