#ifndef WIN_3D_GAME_HELPER_H
#define WIN_3D_GAME_HELPER_H

#include "../../headers/AbstractWinGameHelper.h"
#include "../headers/Win3DGraphicsHelper.h"
#include "Win3DBlock.h"
#include "Win3DBoard.h"

namespace Win3D
{
    class Win3DGameHelper : public Win::AbstractWinGameHelper
    {
    public:
        Win3DGameHelper();
        ~Win3DGameHelper();

        //IGameHelper implementations (the remainder implemented within AbstractWinGameHelper)
        bool initSystemSettings(HINSTANCE hInstance, int nCmdShow);
        void drawBoard(IBoard* board, IBlock* block, int points);

    private:
        Win3DGraphicsHelper* _graphicsHelper;

        //AbstractWinGameHelper implementations
        const wchar_t* getWindowClassName();
        const wchar_t* getWindowTitle();
        void paintWelcomeScreen();
        void paintGameOverScreen();
        void tryDrawCube(Win3DBlock* block, Win3DBoard* board, Point3D coords);
    };
}

#endif
