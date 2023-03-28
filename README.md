3xTetris - An excercise in OOP in C++

A Tetris game logic in Game.cpp uses interfaces defined in Interfaces folder.
Particular implementations of interfaces are provided by 3 classes, according to Abstract Factory pattern:

Console2D::Console2DGameFactory
Win2D::Win2DGameFactory
Win3D::Win3DGameFactory

The whole thing must be run under Windows, as theres wWinMain entry point from WINAPI in main.cpp.
Besides, Direct2D is used to render graphics and windows message queue is checked for keyboard input and paint messages.
In rendering 3D graphics no 3D libraries are used, only pure math.