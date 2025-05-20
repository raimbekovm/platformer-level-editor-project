#include "tile_toolbar.h"
#include <QPushButton>

TileToolbar::TileToolbar(QWidget *parent)
    : QToolBar("Tile Selection", parent)
    , selectedTile(TileType::Air)
{
    buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(true);
    setupButtons();
}

TileToolbar::~TileToolbar() {
}

void TileToolbar::setTileSprites(const QMap<QString, QPixmap>& sprites) {
    tileSprites = sprites;
    // Update button icons
    for (int i = 0; i < buttonGroup->buttons().size(); ++i) {
        QPushButton* button = qobject_cast<QPushButton*>(buttonGroup->buttons()[i]);
        if (button) {
            QString iconPath = button->property("iconPath").toString();
            if (!iconPath.isEmpty() && tileSprites.contains(iconPath)) {
                button->setIcon(tileSprites[iconPath]);
            }
        }
    }
}

char TileToolbar::getSelectedTileType() const {
    return static_cast<char>(selectedTile);
}

void TileToolbar::setupButtons() {
    auto* airButton = createTileButton(TileType::Air, "");
    airButton->setText("Air");
    airButton->setChecked(true);
    addWidget(airButton);
    buttonGroup->addButton(airButton);

    addSeparator();

    addWidget(createTileButton(TileType::Wall, "wall.png"));
    addWidget(createTileButton(TileType::DarkWall, "wall_dark.png"));
    addWidget(createTileButton(TileType::Player, "player.png"));
    addWidget(createTileButton(TileType::Coin, "coin.png"));
    addWidget(createTileButton(TileType::Spikes, "spikes.png"));
    addWidget(createTileButton(TileType::Enemy, "enemy.png"));
    addWidget(createTileButton(TileType::Exit, "exit.png"));
}

QPushButton* TileToolbar::createTileButton(TileType type, const QString& iconPath) {
    QPushButton* button = new QPushButton();
    if (!iconPath.isEmpty()) {
        if (tileSprites.contains(iconPath)) {
            button->setIcon(tileSprites[iconPath]);
        }
        button->setProperty("iconPath", iconPath);
    }
    button->setCheckable(true);
    button->setFixedSize(32, 32);
    button->setIconSize(QSize(24, 24));
    
    // Set tooltip
    QString tooltip;
    switch (type) {
        case TileType::Wall: tooltip = "Wall (#)"; break;
        case TileType::DarkWall: tooltip = "Dark Wall (=)"; break;
        case TileType::Air: tooltip = "Air (space)"; break;
        case TileType::Player: tooltip = "Player (@)"; break;
        case TileType::Coin: tooltip = "Coin (*)"; break;
        case TileType::Spikes: tooltip = "Spikes (^)"; break;
        case TileType::Enemy: tooltip = "Enemy (&)"; break;
        case TileType::Exit: tooltip = "Exit (E)"; break;
    }
    button->setToolTip(tooltip);
    
    connect(button, &QPushButton::clicked, [this, type]() {
        selectedTile = type;
        emit tileSelected(static_cast<char>(type));
    });
    
    buttonGroup->addButton(button);
    return button;
}

QString TileToolbar::getTileIconPath(TileType type) const {
    switch (type) {
        case TileType::Wall: return "wall.png";
        case TileType::DarkWall: return "wall_dark.png";
        case TileType::Player: return "player.png";
        case TileType::Coin: return "coin.png";
        case TileType::Spikes: return "spikes.png";
        case TileType::Enemy: return "enemy.png";
        case TileType::Exit: return "exit.png";
        default: return "";
    }
} 