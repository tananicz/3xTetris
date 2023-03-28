#ifndef WIN_2D_GAME_HELPER_H
#define WIN_2D_GAME_HELPER_H

#include "../../headers/AbstractWinGameHelper.h"
#include "../../../Interfaces/IBlock.h"
#include "../../../Interfaces/IBoard.h"
#include "../headers/Win2DGraphicsHelper.h"

namespace Win2D
{
    class Win2DGameHelper : public Win::AbstractWinGameHelper
    {
    public:
        Win2DGameHelper();
        ~Win2DGameHelper();

        //IGameHelper implementations (the remainder implemented within AbstractWinGameHelper)
        bool initSystemSettings(HINSTANCE hInstance, int nCmdShow);
        void drawBoard(IBoard* board, IBlock* block, int points);

    private:
        static const int _cubeWidth = 25;
        static const int _screenOffsetX = 30;
        static const int _screenOffsetY = 30;
        Win2DGraphicsHelper* _graphicsHelper;
        void paintBordersWithGrid(int boardWidth, int boardHeight, D2D1::ColorF borderColor, D2D1::ColorF gridColor);

        //AbstractWinGameHelper implementations
        const wchar_t* getWindowClassName();
        const wchar_t* getWindowTitle();
        void paintWelcomeScreen();
        void paintGameOverScreen();
    };
}

#endif