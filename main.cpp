#include "Interfaces/IGameFactory.h"
#include "Win/Win2D/headers/Win2DGameFactory.h"
#include "Win/Win3D/headers/Win3DGameFactory.h"
#include "Shared/headers/Game.h"
#include <windows.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    IGameFactory* gameFactory = nullptr;

    if (wcscmp(pCmdLine, L"2d") == 0)
    {
        gameFactory = new Win2D::Win2DGameFactory;
    }
    else
    {
        gameFactory = new Win3D::Win3DGameFactory;
    }

    if (gameFactory->getGameHelperSingleton()->initSystemSettings(hInstance, nCmdShow))
    {
        Game g(gameFactory);
        g.start();
    }

    delete gameFactory;
    gameFactory = nullptr;

    return 0;
}

/*#include "Interfaces/IGameFactory.h"
#include "Console2D/headers/Console2DGameFactory.h"
#include "Shared/headers/Game.h"

int main()
{
    IGameFactory* gameFactory = new Console2D::Console2DGameFactory;

    if (gameFactory->getGameHelperSingleton()->initSystemSettings(0, 0))
    {
        Game g(gameFactory);
        g.start();
    }

    delete gameFactory;
    gameFactory = nullptr;

    return 0;
}*/
