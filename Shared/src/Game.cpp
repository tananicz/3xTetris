#include "../headers/Game.h"

#include "../../Interfaces/IAction.h"
#include "../../Interfaces/IBoard.h"
#include "../../Interfaces/IBlock.h"
#include "../../Interfaces/IActionListener.h"
#include <chrono>
#include "windows.h"

using namespace std;

Game::Game(IGameFactory* gameFactory)
{
    _gameFactory = gameFactory;
}

void Game::start()
{
    do
    {
        IAction* userAction = _gameFactory->getGameHelperSingleton()->getWelcomeScreenAction();
        if (userAction != nullptr)
        {
            bool isStartRequested = userAction->isStartRequested();
            bool isExitRequested = userAction->isExitRequested();
            delete userAction;

            if (isStartRequested)
            {   
                startProperGame();
            }
            else if (isExitRequested)
            {
                break;
            }
        }
    } 
    while (1);
}

void Game::startProperGame()
{
    IBoard* board = _gameFactory->getBoard();
    IBlock* block = nullptr;
    IAction* action = nullptr;
    IGameHelper* gameHelper = _gameFactory->getGameHelperSingleton();
    IActionListener* actionListener = gameHelper->getActionListener();

    double interval = 1;
    int points = 0;
    bool continueMove;

    do
    {
        //first we create user's block and try to place it in center-top position of a board
        block = _gameFactory->getBlock();

        IPosition* initialPos = board->getStartingPosition();
        block->setInitialPosition(initialPos);
        delete initialPos;

        if (!board->canInsertBlock(block))
        {
            delete board;
            delete block;

            IAction* action = _gameFactory->getGameHelperSingleton()->getGameOverScreenAction();
            if (nullptr != action)
                delete action;

            return;
        }
        gameHelper->drawBoard(board, block, points);

        auto startTime = chrono::high_resolution_clock::now();
        continueMove = true;

        //next we start moving user's block down the board and wait for user's actions
        while (continueMove)
        {
            action = actionListener->listenForAction(false);

            if (action != nullptr)
            {
                if (action->isExitRequested())
                {
                    delete action;
                    delete board;
                    delete block;
                    return;
                }

                if (action->isRefreshRequested() || block->tryAction(board, action))
                    gameHelper->drawBoard(board, block, points);
                
                delete action;
                action = nullptr;
            }

            auto iterTime = chrono::high_resolution_clock::now();
            chrono::duration<double> elapsedTime = iterTime - startTime;

            if (elapsedTime.count() > interval)
            {
                startTime = iterTime;

                if (!block->tryPush(board))
                {
                    board->imprintBlock(block);
                    continueMove = false;

                    int collectedRows = board->collectRows();
                    if (collectedRows > 0)
                    {
                        points += (collectedRows * 100);
                        interval = _gameFactory->getGameHelperSingleton()->updateInterval(points);
                        gameHelper->drawBoard(board, block, points);
                    }

                    delete block;
                    block = nullptr;
                }
                else
                    gameHelper->drawBoard(board, block, points);
            }

            Sleep(20);
        }
    } 
    while (1);
}
