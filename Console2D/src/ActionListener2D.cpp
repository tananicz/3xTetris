#include "../headers/ActionListener2D.h"

#include "../headers/ActionEnum.h"
#include "../headers/UserAction2D.h"
#include "windows.h"

namespace Console2D
{
    IAction* ActionListener2D::listenForAction(bool waitForInput)
    {
        int keyStates[] = { VK_LEFT, VK_RIGHT, VK_DOWN, 0x41, 0x5A, VK_ESCAPE, 0x53 };
        ActionEnum actions[] = { MoveLeft, MoveRight, MoveDown, RotateRight, RotateLeft, ExitGame, StartGame };
        int arrLength = sizeof(keyStates) / sizeof(int);

        do
        {
            for (int i = 0; i < arrLength; i++)
            {
                if (GetAsyncKeyState(keyStates[i]))
                {
                    UserAction2D* userAction = new UserAction2D;
                    userAction->actionName = actions[i];
                    return userAction;
                }
            }
            if (!waitForInput)
            {
                return nullptr;
            }
        } while (waitForInput);
    }
}
