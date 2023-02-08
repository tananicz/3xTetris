#ifndef GAME_HELPER_H
#define GAME_HELPER_H

#include "../../Interfaces/IGameHelper.h"
#include "../headers/ColorEnum.h"

using namespace Console2D;
    
namespace Console2D
{
    class GameHelper : public IGameHelper
    {
    public:
        GameHelper();
        ~GameHelper();

        //IGameHelper implementations
        bool initSystemSettings(HINSTANCE hInstance, int nCmdShow);
        IActionListener* getActionListener();
        IAction* getWelcomeScreenAction();
        IAction* getGameOverScreenAction();
        void drawBoard(IBoard* board, IBlock* block, int points);
        double updateInterval(int points);

    private:
        IActionListener* _actionListener;
        int mapColor(ColorEnum c);
    };
}

#endif // GAME_HELPER_H