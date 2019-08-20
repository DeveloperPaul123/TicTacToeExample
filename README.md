# TicTacToeExample
Example of implementing a minimax algorithm for a simple TicTacToe game made for the desktop with Qt. 

## Setup

The only dependency of this project is Qt5. If you are using Visual Studio Code, you'll need to change the path in `.vscode/settings.json` for the `Qt5_DIR` entry to be able to compile the project. 

If you are running cmake manually, you can do the following:

````cmake
cmake -S <source_dir> -B <build_dir> -DQt5_DIR:PATH="<qt5-path>"
cd <build_dir>
cmake --build .
````
