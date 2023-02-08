#ifndef I_GAMEHELPER_H
#define I_GAMEHELPER_H

#include "IAction.h"
#include "IBoard.h"
#include "IBlock.h"
#include "IActionListener.h"
#include <windows.h>

class IGameHelper
{
public:
    virtual ~IGameHelper() = 0 {};
    virtual bool initSystemSettings(HINSTANCE hInstance, int nCmdShow) = 0;
    virtual IActionListener* getActionListener() = 0;
    virtual IAction* getWelcomeScreenAction() = 0;
    virtual IAction* getGameOverScreenAction() = 0;
    virtual void drawBoard(IBoard* board, IBlock* block, int points) = 0;
    virtual double updateInterval(int points) = 0;
};

#endif // I_GAMEHELPER_H
