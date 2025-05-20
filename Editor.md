# Platformer Level Editor Documentation

## Overview
A Qt-based level editor for creating platformer game levels. The editor provides a grid-based interface for placing tiles and basic editing tools.

## Features

### Grid System
- 20x20 grid by default
- Resizable grid (1-100 cells)
- Square cells (32x32 pixels)
- Visual grid lines

### Tiles
- Wall (#)
- Dark Wall (=)
- Player (@)
- Coin (*)
- Spikes (^)
- Enemy (&)
- Exit (E)
- Air (empty space)

### Editing Tools
- Tile placement with left-click
- Undo last action (Ctrl+Z)
- Clear grid (Ctrl+L)
- Resize grid (Ctrl+R)

### File Operations
- Export level to .rll format (Ctrl+S)
  - Uses RLE encoding for compression
  - Basic XOR encryption for security
  - Saves grid dimensions and tile data

### User Interface
- Left toolbar for tile selection
- Top toolbar for editing tools
- Status bar showing current selection and grid position
- Menu bar with File, Edit, and Help menus

## Implementation Details

### Grid Widget
- QTableWidget-based implementation
- Custom cell rendering with icons
- Mouse event handling for tile placement
- Undo stack for action history

### Tile Toolbar
- Button group for tile selection
- Icon-based buttons
- Visual feedback for selected tile

### File Format (.rll)
```
ROWSxCOLS
ENCRYPTED_DATA
```
- First line: Grid dimensions
- Second line: RLE-encoded and encrypted tile data

### RLE Encoding
- Compresses repeated characters
- Format: [count][character]
- Example: "3#2 " means "###  "

### Encryption
- Simple XOR encryption with a key
- Basic security for level data

## Keyboard Shortcuts
- Ctrl+S: Save/Export level
- Ctrl+Z: Undo
- Ctrl+L: Clear grid
- Ctrl+R: Resize grid
- F1: Show this documentation 