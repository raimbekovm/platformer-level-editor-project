#include "main_window.h"
#include "utilities.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), selectedTile(TileType::Air)
{
    // TODO
}

MainWindow::~MainWindow() { }

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // TODO

    QMainWindow::keyPressEvent(event);
}

void MainWindow::selectTile(char tile)
{
    // TODO
}

void MainWindow::onTileClicked(int row, int col)
{
    // TODO
    // item = currentLevel()->item(row, col);
    // item->setData(Qt::UserRole, tile);
}

void MainWindow::undoTilePlacement()
{
    if (undoStack.isEmpty()) return;
    TileAction action = undoStack.pop();

    // TODO
}

QPushButton* MainWindow::createButton(const QIcon &icon, std::function<void()> action)
{
    QPushButton *button = new QPushButton();

    // TODO

    return button;
}

void MainWindow::clearLevel()
{
    // TODO
}

void MainWindow::resizeLevel()
{
    QDialog resizeDialog(this);
    // TODO
    if (resizeDialog.exec() == QDialog::Accepted) {
        // TODO
    }
}

void MainWindow::exportToFile()
{
    QString filePath = QFileDialog::getSaveFileName(
        this,
        "Export Level",
        "",
        "RLL Files (*.rll);;All Files (*)"
    );

    QFile file(filePath);
    QTextStream out(&file);

    // TODO

    /* Snippets:
     * std::vector<char> data(rows * cols, '-');
     * char tile = item->data(Qt::UserRole).toChar().toLatin1();
     * encrypt(rows, cols, data.data(), output);
     * out << output;
     */

    file.close();
}
