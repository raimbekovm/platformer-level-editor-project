#include "main_window.h"
#include "utilities.h"
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
#include <QKeySequence>
#include <QFile>
#include <QTextStream>
#include <QTextEdit>
#include <QDataStream>
#include <QFileInfo>
#include <QSettings>

// Main window initialization and setup
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

// Core UI setup with grid and toolbars
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

// Load and scale tile sprites from data directory
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

// Setup edit toolbar with clear, resize, and undo actions
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

// Setup main menu bar with file, edit, and help menus
void MainWindow::setupMenuBar() {
    // File Menu
    auto* fileMenu = menuBar()->addMenu("File");
    
    auto* exportAction = new QAction("Export Level", this);
    exportAction->setShortcut(QKeySequence::Save);
    connect(exportAction, &QAction::triggered, this, &MainWindow::exportLevel);
    fileMenu->addAction(exportAction);
    
    fileMenu->addSeparator();
    
    auto* exitAction = new QAction("Exit", this);
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    fileMenu->addAction(exitAction);

    // Edit Menu
    auto* editMenu = menuBar()->addMenu("Edit");
    
    auto* clearAction = new QAction("Clear Level", this);
    clearAction->setShortcut(QKeySequence("Ctrl+L"));
    connect(clearAction, &QAction::triggered, this, &MainWindow::onClearGrid);
    editMenu->addAction(clearAction);
    
    auto* resizeAction = new QAction("Resize Level", this);
    resizeAction->setShortcut(QKeySequence("Ctrl+R"));
    connect(resizeAction, &QAction::triggered, this, &MainWindow::onResizeGrid);
    editMenu->addAction(resizeAction);
    
    editMenu->addSeparator();
    
    auto* undoAction = new QAction("Undo", this);
    undoAction->setShortcut(QKeySequence::Undo);
    connect(undoAction, &QAction::triggered, this, &MainWindow::onUndo);
    editMenu->addAction(undoAction);

    // Help Menu
    auto* helpMenu = menuBar()->addMenu("Help");
    
    auto* documentationAction = new QAction("Documentation", this);
    documentationAction->setShortcut(QKeySequence("F1"));
    connect(documentationAction, &QAction::triggered, this, &MainWindow::showDocumentation);
    helpMenu->addAction(documentationAction);
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

// Handle tile selection from toolbar
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

// Handle tile placement in grid
void MainWindow::onTilePlaced(int row, int col, const QString& type) {
    updateStatusBar(row, col, type);
}

// Clear the entire grid
void MainWindow::onClearGrid() {
    gridWidget->clearGrid();
    statusBar()->showMessage("Grid cleared", 2000);
}

// Resize grid dialog and handling
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

// Undo last tile placement
void MainWindow::onUndo() {
    if (gridWidget->undoLastAction()) {
        statusBar()->showMessage("Undo successful", 2000);
        } else {
        statusBar()->showMessage("Nothing to undo", 2000);
    }
}

// Get current grid data as string
QString MainWindow::getLevelData() const {
    QString data;
    for (int row = 0; row < gridWidget->rowCount(); ++row) {
        for (int col = 0; col < gridWidget->columnCount(); ++col) {
            QTableWidgetItem* item = gridWidget->item(row, col);
            data += item ? item->text() : " ";
        }
    }
    return data;
}

// Export level with RLE encoding and encryption
void MainWindow::exportLevel() {
    QString fileName = QFileDialog::getSaveFileName(this,
        "Export Level", "", "RLL Files (*.rll)");
    
    if (fileName.isEmpty()) return;
    
    if (!fileName.endsWith(".rll")) {
        fileName += ".rll";
    }
    
    // Get level data and encode it
    QString levelData = getLevelData();
    QString encodedData = Utilities::encodeRLE(levelData);
    QString encryptedData = Utilities::encrypt(encodedData, "LEVEL_KEY");
    
    // Save to file
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << gridWidget->rowCount() << "x" << gridWidget->columnCount() << "\n";
        out << encryptedData;
        file.close();
        
        statusBar()->showMessage("Level exported successfully", 2000);
    } else {
        QMessageBox::warning(this, "Export Error",
            "Could not save the level file.");
    }
}

// Create new empty level
void MainWindow::newLevel()
{
    // Clear the current grid
    gridWidget->clearGrid();
    currentFilePath.clear();
    setWindowTitle("Level Editor - New Level");
}

// Open existing level file
void MainWindow::openLevel(const QString& path)
{
    QString filePath = path;
    if (filePath.isEmpty()) {
        filePath = QFileDialog::getOpenFileName(this,
            "Open Level", "", "Level Files (*.level);;All Files (*)");
    }

    if (!filePath.isEmpty()) {
    QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            QDataStream in(&file);
            in.setVersion(QDataStream::Qt_6_0);

            // Read grid dimensions
            int rows, cols;
            in >> rows >> cols;

            // Resize grid
            gridWidget->resizeGrid(rows, cols);

            // Read grid data
            for (int row = 0; row < rows; ++row) {
                for (int col = 0; col < cols; ++col) {
                    char tileType;
                    in >> tileType;
                    gridWidget->placeTile(row, col, tileType);
                }
            }

    file.close();
            currentFilePath = filePath;
            setWindowTitle("Level Editor - " + QFileInfo(filePath).fileName());
            addToRecentFiles(filePath);
        } else {
            QMessageBox::warning(this, "Error", "Could not open file for reading.");
        }
    }
}

// Manage recent files list
void MainWindow::addToRecentFiles(const QString& filePath)
{
    QSettings settings;
    QStringList recentFiles = settings.value("recentFiles").toStringList();
    
    // Remove the file if it's already in the list
    recentFiles.removeAll(filePath);
    
    // Add to the beginning of the list
    recentFiles.prepend(filePath);
    
    // Keep only the 10 most recent files
    while (recentFiles.size() > 10) {
        recentFiles.removeLast();
    }
    
    settings.setValue("recentFiles", recentFiles);
    updateRecentFilesMenu();
}

// Update recent files menu
void MainWindow::updateRecentFilesMenu()
{
    if (!recentFilesMenu) {
        recentFilesMenu = new QMenu("Recent Files", this);
        menuBar()->findChild<QMenu*>("File")->addMenu(recentFilesMenu);
    }
    
    recentFilesMenu->clear();
    
    QSettings settings;
    QStringList recentFiles = settings.value("recentFiles").toStringList();
    
    for (const QString& file : recentFiles) {
        QAction* action = recentFilesMenu->addAction(QFileInfo(file).fileName());
        connect(action, &QAction::triggered, [this, file]() {
            openLevel(file);
        });
    }
}

// Display documentation from Editor.md
void MainWindow::showDocumentation()
{
    // Try to find the documentation file in different locations
    QStringList possiblePaths = {
        "Editor.md",  // Current directory
        "../Editor.md",  // Parent directory
        "data/Editor.md",  // Data directory
        "../data/Editor.md"  // Parent data directory
    };
    
    QFile file;
    for (const QString& path : possiblePaths) {
        file.setFileName(path);
        if (file.exists()) {
            break;
        }
    }
    
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString content = in.readAll();
        file.close();
        
        // Create a dialog to display the documentation
        auto* dialog = new QDialog(this);
        dialog->setWindowTitle("Level Editor Documentation");
        dialog->resize(600, 400);
        
        auto* layout = new QVBoxLayout(dialog);
        
        auto* textEdit = new QTextEdit(dialog);
        textEdit->setReadOnly(true);
        textEdit->setMarkdown(content);
        layout->addWidget(textEdit);
        
        auto* buttonBox = new QDialogButtonBox(
            QDialogButtonBox::Close,
            Qt::Horizontal, dialog);
        layout->addWidget(buttonBox);
        
        connect(buttonBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);
        
        dialog->exec();
        dialog->deleteLater();
    } else {
        QMessageBox::warning(this, "Documentation Error",
            "Could not open the documentation file.\n"
            "Please make sure Editor.md exists in the program directory.");
    }
}

// Save current level to file
void MainWindow::saveLevel()
{
    if (currentFilePath.isEmpty()) {
        QString fileName = QFileDialog::getSaveFileName(this,
            "Save Level", "", "Level Files (*.level);;All Files (*)");
        if (fileName.isEmpty()) return;
        currentFilePath = fileName;
    }
    
    QFile file(currentFilePath);
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_6_0);
        
        // Write grid dimensions
        out << gridWidget->rowCount() << gridWidget->columnCount();
        
        // Write grid data
        for (int row = 0; row < gridWidget->rowCount(); ++row) {
            for (int col = 0; col < gridWidget->columnCount(); ++col) {
                QTableWidgetItem* item = gridWidget->item(row, col);
                char tileType = item ? item->text()[0].toLatin1() : ' ';
                out << tileType;
            }
        }
        
        file.close();
        setWindowTitle("Level Editor - " + QFileInfo(currentFilePath).fileName());
        addToRecentFiles(currentFilePath);
        statusBar()->showMessage("Level saved successfully", 2000);
    } else {
        QMessageBox::warning(this, "Save Error",
            "Could not save the level file.");
    }
}

// Display about dialog
void MainWindow::showAbout()
{
    QMessageBox::about(this, "About Level Editor",
        "Level Editor v1.0\n\n"
        "A simple level editor for platformer games.\n\n"
        "Features:\n"
        "- Grid-based level editing\n"
        "- Multiple tile types\n"
        "- Undo/Redo support\n"
        "- Level export/import\n"
        "- Documentation support");
}