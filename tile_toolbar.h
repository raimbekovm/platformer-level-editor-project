#ifndef TILE_TOOLBAR_H
#define TILE_TOOLBAR_H

#include <QToolBar>
#include <QButtonGroup>
#include <QMap>
#include <QPixmap>
#include <QPushButton>

class TileToolbar : public QToolBar
{
    Q_OBJECT

public:
    explicit TileToolbar(QWidget *parent = nullptr);
    ~TileToolbar();

    void setTileSprites(const QMap<QString, QPixmap>& sprites);
    char getSelectedTileType() const;

signals:
    void tileSelected(char type);

private:
    enum class TileType
    {
        Coin     = '*',
        Enemy    = '&',
        Exit     = 'E',
        Player   = '@',
        Spikes   = '^',
        Wall     = '#',
        DarkWall = '=',
        Air      = ' '
    };

    void setupButtons();
    QPushButton* createTileButton(TileType type, const QString& iconPath);
    QString getTileIconPath(TileType type) const;

    QButtonGroup* buttonGroup;
    QMap<QString, QPixmap> tileSprites;
    TileType selectedTile;
};

#endif // TILE_TOOLBAR_H 