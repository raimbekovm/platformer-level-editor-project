#include "main_window.h"
#include <QVBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QSpinBox>
#include <QFormLayout>
#include <QDialog>
#include <QDialogButtonBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    loadTileSprites();
    setupUI();
    setupMenuBar();
    setupStatusBar();
}

MainWindow::~MainWindow() {
}

void MainWindow::setupUI() {
    auto* centralWidget = new QWidget(this);
    auto* layout = new QVBoxLayout(centralWidget);
    
    gridWidget = new GridWidget(this);
    layout->addWidget(gridWidget);
    
    tileToolbar = new TileToolbar(this);
    addToolBar(Qt::LeftToolBarArea, tileToolbar);
    
    setupEditToolbar();
    
    gridWidget->setTileSprites(tileSprites);
    tileToolbar->setTileSprites(tileSprites);
    
    connect(tileToolbar, &TileToolbar::tileSelected, this, &MainWindow::onTileSelected);
    connect(gridWidget, &GridWidget::tilePlaced, this, &MainWindow::onTilePlaced);
    
    setCentralWidget(centralWidget);
    resize(800, 600);
}

void MainWindow::loadTileSprites() {
    QStringList spriteFiles = {
        "wall.png", "wall_dark.png", "player.png",
        "coin.png", "spikes.png", "enemy.png", "exit.png"
    };
    
    QString spritePath = "data/sprites/";
    for (const QString& file : spriteFiles) {
        QString fullPath = spritePath + file;
        QPixmap pixmap(fullPath);
        if (!pixmap.isNull()) {
            tileSprites[file] = pixmap.scaled(28, 28, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        }
    }
}

void MainWindow::setupEditToolbar() {
    editToolBar = new QToolBar("Edit Tools", this);
    addToolBar(Qt::TopToolBarArea, editToolBar);
    
    auto* clearAction = new QAction("Clear Grid", this);
    connect(clearAction, &QAction::triggered, this, &MainWindow::onClearGrid);
    editToolBar->addAction(clearAction);
    
    auto* resizeAction = new QAction("Resize Grid", this);
    connect(resizeAction, &QAction::triggered, this, &MainWindow::onResizeGrid);
    editToolBar->addAction(resizeAction);
    
    auto* undoAction = new QAction("Undo", this);
    connect(undoAction, &QAction::triggered, this, &MainWindow::onUndo);
    editToolBar->addAction(undoAction);
}

void MainWindow::setupMenuBar() {
    auto* fileMenu = menuBar()->addMenu("File");
    
    auto* exportAction = new QAction("Export Level", this);
    connect(exportAction, &QAction::triggered, [this]() {
        QString fileName = QFileDialog::getSaveFileName(this,
            "Export Level", "", "Text Files (*.txt)");
        if (!fileName.isEmpty()) {
            QMessageBox::information(this, "Export", "Level export not implemented yet");
        }
    });
    fileMenu->addAction(exportAction);
    
    fileMenu->addSeparator();
    
    auto* exitAction = new QAction("Exit", this);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    fileMenu->addAction(exitAction);
}

void MainWindow::setupStatusBar() {
    statusLabel = new QLabel(this);
    statusBar()->addWidget(statusLabel);
    updateStatusBar(-1, -1, "");
}

void MainWindow::updateStatusBar(int row, int col, const QString& type) {
    if (row < 0 || col < 0) {
        statusLabel->setText("Ready");
    } else {
        statusLabel->setText(QString("Row: %1, Col: %2, Tile: %3")
            .arg(row).arg(col).arg(type));
    }
}

void MainWindow::onTileSelected(char type) {
    QString tileType;
    switch (type) {
        case '#': tileType = "Wall"; break;
        case '=': tileType = "Dark Wall"; break;
        case ' ': tileType = "Air"; break;
        case '@': tileType = "Player"; break;
        case '*': tileType = "Coin"; break;
        case '^': tileType = "Spikes"; break;
        case '&': tileType = "Enemy"; break;
        case 'E': tileType = "Exit"; break;
        default: tileType = "Unknown";
    }
    statusBar()->showMessage("Selected: " + tileType);
    gridWidget->setCurrentTileType(type);
}

void MainWindow::onTilePlaced(int row, int col, const QString& type) {
    updateStatusBar(row, col, type);
}

void MainWindow::onClearGrid() {
    gridWidget->clearGrid();
    statusBar()->showMessage("Grid cleared", 2000);
}

void MainWindow::onResizeGrid() {
    auto* dialog = new QDialog(this);
    dialog->setWindowTitle("Resize Grid");
    
    auto* layout = new QFormLayout(dialog);
    
    auto* rowsSpinBox = new QSpinBox(dialog);
    rowsSpinBox->setRange(1, 100);
    rowsSpinBox->setValue(gridWidget->rowCount());
    layout->addRow("Rows:", rowsSpinBox);
    
    auto* colsSpinBox = new QSpinBox(dialog);
    colsSpinBox->setRange(1, 100);
    colsSpinBox->setValue(gridWidget->columnCount());
    layout->addRow("Columns:", colsSpinBox);
    
    auto* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, dialog);
    layout->addRow(buttonBox);
    
    connect(buttonBox, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);
    
    if (dialog->exec() == QDialog::Accepted) {
        gridWidget->resizeGrid(rowsSpinBox->value(), colsSpinBox->value());
        statusBar()->showMessage(QString("Grid resized to %1x%2")
            .arg(rowsSpinBox->value())
            .arg(colsSpinBox->value()), 2000);
    }
    
    dialog->deleteLater();
}

void MainWindow::onUndo() {
    if (gridWidget->undoLastAction()) {
        statusBar()->showMessage("Undo successful", 2000);
    } else {
        statusBar()->showMessage("Nothing to undo", 2000);
    }
}