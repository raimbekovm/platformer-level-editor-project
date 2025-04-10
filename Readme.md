<h1 align=center>COM-119 2025S Project 2: platformer-level-editor</h1>

<h5 align=center>This is an extension of <a href="https://github.com/toksaitov/platformer-project">Project 1</a> of the Object-Oriented Programming course at AUCA </h5>

![Simple Graphics Editor](https://i.imgur.com/UAmqni6.png)

<p align=right>This is a <a href="https://drive.google.com/file/d/1s67k72nAoKYe_W6Gvi6jgH-63cm4_86F/view?usp=sharing">potential implementation</a> of Project 2. Yours may differ.</p>

---

The primary goal of this project is to develop a level editor for [Project 1](https://github.com/toksaitov/platformer-project) levels in C++ using the [Qt6](https://www.qt.io/product/qt6) framework following both the object-oriented and event-driven paradigms to build an extendable application. The project will allow you to explore graphical user interface development, event handling, and serve as testing grounds for advanced OOP concepts—inheritance, polymorphism, and encapsulation.

To get started, clone the repository to get baseline project code. You are encouraged to modify and refactor it as needed to meet the project requirements.

Be inventive in applying object-oriented and event-driven principles to design the application, ensuring to implement the requirements*:

<h5>* Note: <i>you <u>must</u> utilize the Qt6 framework for <u>all</u> aspects of the user interface and graphics processing.</i></h5>

## Implementation Requirements

To complete this project, you are required to: implement all of the **basic features** (highlighted in bold); and implement three or more *advanced features* (highlighted in italic w/ the keyword "advanced"). The feature set for the level editor can be found below:

### Application design

- **Grid editor**: You must implement a table to store items in a grid-based layout. It is recommended to use a default Qt class (QTableWidget/QGraphicsGridLayout—refer to documentation before settling on either), although a custom widget may be created. The grid-based layout must display sprites on tiles (you may find them at `./data/sprites/` or use custom-made ones—however, they must match the ones in your first project). The user must be able to place tiles onto the grid-based layout with a mouse click.

- **Tile selection panel**: You must implement a way to select the type of tile which the user will be able to place onto the grid-based layout. It is recommended to use `QToolbar` that parents a Qt box layout to host a multitude of [factory](https://refactoring.guru/design-patterns/factory-method)-produced `QPushButton`s.

- (advanced) *Drag-placing Tiles*: You could implement the ability to drag-place tiles onto the grid-based layout. Depending on which class you are using for the grid-based layout, you might need to overload member functions to implement work-arounds.

- (advanced) *Dragging Structures*: You could implement the ability to select a range of tiles and move them around the grid-based layout. Once again, you might need to employ work-arounds to allow for such functionality.

- **Basic editing tools**: You must implement a way to: **clear** the grid-based layout, **resize** the grid-based layout, and **undo tile placements** from the grid-based layout. The tools should be placed on a Qt box layout, that may be put onto another `QToolbar`.

- **Menu bar**: You must implement a menu bar that will allow the user to (at least): export their project, close all windows, clear the selected level, resize the selected level, undo tile placements, and access documentation. It is strongly advised to use a `QMenuBar` for this task.

- (advanced) *Multiple-level handling*: You could, within your program, implement the capability to handle multiple levels, moving between those levels at will and retaining the data of all existing levels, even when they're not displayed.

  - (advanced) *Advanced editing tools*: If you implement **multiple-level handling** you need to implement a way to: add a new level, delete the current level, move to the next level, move to the previous level, and know which level you are on. You should host these tools in the same place you host your **basic editing tools** at.

### Handling I/O

- **Export functionality**: You must implement a way to export the grid-based layout into an `.rll` file, utilizing [RLE encoding](#run-length-encoding). Collapse your grid-based layout into a QString, and put it through an encryption function. The encryption function is provided in `utilities.h`. The user must be able to specify the export location, preferably using a `QFileDialog`.

- (advanced) *Import functionality & Error-handling*: You could implement a way to reconstruct your grid-based layout(s) from a user-selected `.rll` file. You must also implement error checking and handling of corrupted `.rll` files. You may utilize your decryption algorithm from the first project.

### Improving UX

- **Documentation**: You must provide a guide to your program (the `Editor.md` file) that is accessible from within your program. You may simply utilize a `QDialog` for a pop-up window.

- (advanced) *Splash and Welcome screen and Style sheets usage*: You could make your program more appealing by: designing and implementing a **splash screen**, implementing a **welcome screen**, and redesigning UI elements with **style sheets**. However, keep in mind that while the splash screen is active, no other window should be visible. It should be followed by the welcome screen, and only after the user decides what to do should the main window appear.

### Additional Features

- (advanced) *Additional level element tiles*: You could add __three or more__ additional level element tiles to your level editor and `.<custom_file_extension>` files (keys, etc.). Keep in mind that your Project 1 must be able to handle the additional elements in the `.<custom_file_extension>` files.

- (full-points) *Collaborative editing*: You are allowed to attempt to implement collaborative over-the-network level editing.

\* Keep in mind that an addition of a feature will affect existing ones. For instance, choosing to implement *Multiple-level handling* means that **Export functionality** will be affected, as the program will need to export __multiple levels__ (instead of just a single one) into a single `.rll` file.

## Additional Requirements:

- **Code Organization**: Maintain a clean and organized codebase. Structure your project into appropriate directories and files, ensuring a logical separation of classes and assets.

- **Design Requirements**: Ensure your application is well-designed and stylistically logical and consistent. Group tools according to their purpose, breaking them into different layouts to guarantee a user-friendly environment.

- **Readme.md**: Create an `Editor.md` file detailing your implementation, including a description of how you approached the implementation process, any challenges faced, and how you overcame them. Also, document the steps required to set up and run your version of the game.

- **Commit History**: Develop your project with a detailed commit history in a version control system. Regular commits with descriptive messages are essential to demonstrate the progression and milestones of your project work.

## Deliverables

* Develop the editor within the `<repository>` directory. This directory should include a `data` subdirectory containing all assets (icons, sprits, sounds, fonts, etc.). Additionally, ensure the `<repository>` directory contains an `Editor.md` file detailing your game and listing the implemented features.

* Organize your code into appropriate header files and classes. Each header file should contain related function definitions.

* Your `main.cpp` should follow standard practices for large Qt applications.

Upon completion, your project directory should have an organized structure similar to the following example:

```
.
└── <repository>
    ├── data
    │   ├── icons
    │   ├── sprites
    │   └── ...
    ├── main.cpp
    ├── mainwindow.h
    ├── mainwindow.cpp
    ├── utilities.h
    ├── <various header files>.h
    ├── ... (.idea, .gitignore, CMakeLists.txt, and other directories)
    ├── Editor.md
    └── Readme.md
```

Commit and push your work at regular intervals, such as daily or after significant changes. Finally, submit the URL of the last commit to Moodle before the deadline.

<h1 align="center"> Attachments </h1>

## Installing Qt 6

To develop this project, you will need to have the Qt library (pronounced \[kjuːt\]) installed on your system. To do so, you must follow the instructions below to install the library for your operating system. Alternatively, you may use our labs (which have the library installed) to work on this project.

### Windows

1. Download the following [archive](https://drive.google.com/file/d/14iCNNsdpZTj4t9ZNz4Mdi79Hd6pWwuab/view) and extract it to the `C:` drive. Ensure that after extraction there is a directory named `Qt` (`C:\Qt`), and inside it, a directory named `6.8.2` (`C:\Qt\6.8.2`). If you do not trust our package, you can install Qt 6 yourself using the official online [installer](https://www.qt.io/download-open-source). Select only the library for the MSVC compiler, version `6.8.2`; do not install any additional components, as the full installation requires significant disk space.
2. Run PowerShell as an administrator. Once open, execute the code below to set global OS variables that will help build tools locate your Qt 6 installation:

```powershell
[System.Environment]::SetEnvironmentVariable("Qt6_DIR", "C:\Qt\6.8.2\msvc2022_64", "Machine")

$currentPath = [System.Environment]::GetEnvironmentVariable("Path", "Machine")
$newDir = "C:\Qt\6.8.2\msvc2022_64\bin"
if ($currentPath -notlike "*$newDir*") {
    $updatedPath = "$currentPath;$newDir"
    [System.Environment]::SetEnvironmentVariable("Path", $updatedPath, "Machine")
}
```

### macOS

1. Install or update the [Homebrew](https://brew.sh) package manager.
2. Install Qt 6 with Homebrew by running `brew install qt6` in the Terminal application.

### Ubuntu

1. Install Qt 6 with the system package manager by running `sudo apt install qt6-base-dev` in the Terminal application.

## Run-Length Encoding

This project utilizes RLE to encode levels, increasing compactness and efficiency. Level elements are represented by the following elements:

| Puzzle Element | Character | ASCII code |
|:---------------|:----------|:-----------|
| Wall           | #         | 0x23       |
| Dark Wall      | =         | 0x3D       |
| Air            | -         | 0x2D       |
| Player         | @         | 0x40       |
| Coin           | *         | 0x2A       |
| Spike          | ^         | 0x5E       |
| Enemy          | &         | 0x26       |

In the RLE format, digits are used to show how many elements of the same type are following; as such:

`---- becomes 4-`.

So the following level,
```
 -------
 ---*---
 -------
 -------
 -------
 @-^^^-E
 #######
```

looks like this with RLE:

`7-|3-*3-|7-|7-|7-|@-3^-E|7#`.

The rows of the level are separated by vertical bars (`|`s).

## Implementation Tips

- Lay out everything you would like to implement before starting work on the project—changing course in the middle of a project will not end well.
- Wherever possible, use default Qt widgets to achieve the objective.
- Do not put every function you use in `mainwindow.cpp` into `mainwindow.h`.
- Read the documentation of widgets you are using.
- Spend time searching for ways to break your program: that will help you eliminate edge cases and bugs.
- Use compilation and linking errors to your advantage: _read_ them.
- Use the debug tool to resolve crashes.
- If something isn't working as it should—read documentation.

## Deadline

Refer to the specific course or assignment page on Moodle for detailed information about deadlines.

## Links

### Qt

* [Qt 6](https://www.qt.io/product/qt6)
* [Examples](https://doc.qt.io/qt-6/qtexamplesandtutorials.html)
* [Documentation](https://doc.qt.io/)

### Tools

* [krita](https://krita.org/en/)

## Books

* Introduction to Programming with C++, 3rd Edition by Daniel Liang

## Credits

* [Luna Maltseva](https://github.com/lunamaltseva)
