#include "../headers/GameHelper.h"

#include "../headers/ActionEnum.h"
#include "../headers/ActionListener2D.h"
#include "../headers/TextBlock.h"
#include "../headers/TextBoard.h"
#include "../headers/UserAction2D.h"
#include <iostream>
#include <stdlib.h>
#include "windows.h"

using namespace std;

namespace Console2D
{
    GameHelper::GameHelper()
    {
        this->_actionListener = new ActionListener2D;
    }

    GameHelper::~GameHelper()
    {
        delete this->_actionListener;
        this->_actionListener = nullptr;
    }

    bool GameHelper::initSystemSettings(HINSTANCE hInstance, int nCmdShow)
    {
        return true;
    }

    double GameHelper::updateInterval(int points)
    {
        int factor = points / 300;
        return 1 - (factor * 0.1);
    }

    IAction* GameHelper::getWelcomeScreenAction()
    {
        system("cls");

        cout << "****************************************" << endl;
        cout << "**       WELCOME TO TETRIS GAME       **" << endl;
        cout << "****************************************" << endl;
        cout << "** S - Start                          **" << endl;
        cout << "** Esc - Exit                         **" << endl;
        cout << "****************************************" << endl;

        UserAction2D* action = nullptr;
        do
        {
            action = static_cast<UserAction2D*>(_actionListener->listenForAction(true));
        } while ((action->actionName != StartGame) && (action->actionName != ExitGame));

        return action;
    }

    IAction* GameHelper::getGameOverScreenAction()
    {
        cout << "****************************" << endl;
        cout << "**     GAME OVER MAN!     **" << endl;
        cout << "**                        **" << endl;
        cout << "**     Esc - Exit         **" << endl;
        cout << "****************************" << endl;

        UserAction2D* action = nullptr;
        do
        {
            action = static_cast<UserAction2D*>(_actionListener->listenForAction(true));
        } while (action->actionName != ExitGame);

        return action;
    }

    IActionListener* GameHelper::getActionListener()
    {
        return _actionListener;
    }

    void GameHelper::drawBoard(IBoard* board, IBlock* block, int points)
    {
        TextBlock* textBlock = static_cast<TextBlock*>(block);
        TextBoard* textBoard = static_cast<TextBoard*>(board);

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, mapColor(White));
        system("cls");

        for (int y = 0; y < textBoard->getHeight(); y++)
        {
            cout << "|";

            for (int x = 0; x < textBoard->getWidth(); x++)
            {
                if (textBoard->isPositionInsideBlock(textBlock, x, y))
                {
                    SetConsoleTextAttribute(hConsole, mapColor(textBlock->getColor()));
                    cout << "#";
                    SetConsoleTextAttribute(hConsole, mapColor(White));
                }
                else
                {
                    ColorEnum c = textBoard->getColorEnumAtPos(x, y); //_board[x][y];
                    if (c != None)
                    {
                        SetConsoleTextAttribute(hConsole, mapColor(c));
                        cout << "#";
                        SetConsoleTextAttribute(hConsole, mapColor(White));
                    }
                    else
                        cout << " ";
                }
            }

            cout << "|" << endl;
        }

        for (int x = 1; x <= textBoard->getWidth() + 2; x++)
            cout << "=";

        cout << endl << endl << "   Points: " << points << endl;
    }

    int GameHelper::mapColor(ColorEnum c)
    {
        switch (c)
        {
        case White:
            return 15;
            break;
        case Yellow:
            return 14;
            break;
        case Red:
            return 12;
            break;
        case Green:
            return 10;
            break;
        case Blue:
            return 9;
            break;
        default:
            return 15;
            break;
        }
    }
}
