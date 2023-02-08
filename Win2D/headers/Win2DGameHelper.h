#ifndef WIN_2D_GAME_HELPER_H
#define WIN_2D_GAME_HELPER_H

#include "../../Interfaces/IGameHelper.h"
#include "../../Interfaces/IBlock.h"
#include "../../Interfaces/IBoard.h"
#include "../headers/Win2DGraphicsHelper.h"

namespace Win2D
{
    class Win2DGameHelper : public IGameHelper
    {
    public:
        Win2DGameHelper();
        ~Win2DGameHelper();

        //IGameHelper implementations
        bool initSystemSettings(HINSTANCE hInstance, int nCmdShow);
        IActionListener* getActionListener();
        IAction* getWelcomeScreenAction();
        IAction* getGameOverScreenAction();
        void drawBoard(IBoard* board, IBlock* block, int points);
        double updateInterval(int points);

    private:
        HINSTANCE _hInstance;
        HWND _windowHandle;
        IActionListener* _actionListener;
        Win2DGraphicsHelper* _graphicsHelper;
        static const int _cubeWidth = 25;
        static const int _screenOffsetX = 30;
        static const int _screenOffsetY = 30;
        void paintWelcomeScreen();
        void paintGameOverScreen();
        void paintBorders(int boardWidth, int boardHeight, D2D1::ColorF color);
        wchar_t* intToWcharTPtr(int i);
        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    };
}

#endif