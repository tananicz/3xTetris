#include "../headers/TextBlock.h"

#include "../../Interfaces/IAction.h"
#include "../headers/TextBoard.h"
#include "../headers/UserAction2D.h"
#include <stdlib.h>

namespace Console2D
{
    TextBlock::TextBlock()
    {
        for (int i = 0; i < 4; i++)
        {
            _coords[i].x = 0;
            _coords[i].y = 0;
        }
        _color = (ColorEnum)(rand() % 5 + 1);
        setCoords();
    }

    ColorEnum TextBlock::getColor()
    {
        return _color;
    }

    int TextBlock::getBlockSize()
    {
        return _cubeCount;
    }

    Point TextBlock::getPositionAt(int i)
    {
        return _coords[i];
    }

    void TextBlock::setInitialPosition(IPosition* position)
    {
        Point* p = static_cast<Point*>(position);

        int minX, maxX;
        minX = maxX = _coords[0].x;

        for (int i = 1; i < _cubeCount; i++)
        {
            if (_coords[i].x < minX)
                minX = _coords[i].x;
            if (_coords[i].x > maxX)
                maxX = _coords[i].x;
        }

        int blockWidth = maxX - minX + 1;

        Point offset;
        offset.x = p->x - blockWidth / 2;
        offset.y = 0;

        for (int i = 0; i < _cubeCount; i++)
            _coords[i].moveBy(offset);
    }

    bool TextBlock::tryPush(IBoard* board)
    {
        UserAction2D userAction;
        userAction.actionName = MoveDown;
        return tryAction(board, &userAction);
    }

    bool TextBlock::tryAction(IBoard* board, IAction* action)
    {
        TextBoard* textBoard = static_cast<TextBoard*>(board);
        UserAction2D* userAction = static_cast<UserAction2D*>(action);

        if (userAction->actionName == MoveDown || userAction->actionName == MoveLeft || userAction->actionName == MoveRight)
        {
            return tryMove(textBoard, userAction);
        }
        else if (userAction->actionName == RotateLeft || userAction->actionName == RotateRight)
        {
            return tryRotate(textBoard, userAction);
        }
        else
            return false;
    }

    bool TextBlock::tryMove(TextBoard* board, UserAction2D* action)
    {
        Point offset;

        switch (action->actionName)
        {
        case MoveRight:
            offset.x = 1;
            offset.y = 0;
            break;
        case MoveLeft:
            offset.x = -1;
            offset.y = 0;
            break;
        case MoveDown:
            offset.x = 0;
            offset.y = 1;
            break;
        default:
            offset.x = 0;
            offset.y = 0;
            break;
        }

        for (int i = 0; i < _cubeCount; i++)
            _coords[i].moveBy(offset);

        if (board->isCollision(this))
        {
            offset.x = -1 * offset.x;
            offset.y = -1 * offset.y;

            for (int i = 0; i < _cubeCount; i++)
                _coords[i].moveBy(offset);

            return false;
        }

        return true;
    }

    bool TextBlock::tryRotate(TextBoard* board, UserAction2D* action)
    {
        if (action->actionName == RotateLeft || action->actionName == RotateRight)
        {
            performRotation(action);
            if (board->isCollision(this))
            {
                reverseRotationAction(action);
                performRotation(action);
                return false;
            }
            return true;
        }
        else
            return false;
    }

    void TextBlock::performRotation(UserAction2D* action)
    {
        Point offset = _coords[_centerBlock];

        offset.x = -1 * offset.x;
        offset.y = -1 * offset.y;

        for (int i = 0; i < _cubeCount; i++)
            _coords[i].moveBy(offset);

        for (int i = 0; i < _cubeCount; i++)
        {
            int tmp = _coords[i].x;

            if (action->actionName == RotateLeft)
            {
                _coords[i].x = _coords[i].y;
                _coords[i].y = -1 * tmp;
            }
            else if (action->actionName == RotateRight)
            {
                _coords[i].x = -1 * _coords[i].y;
                _coords[i].y = tmp;
            }
        }

        offset.x = -1 * offset.x;
        offset.y = -1 * offset.y;

        for (int i = 0; i < _cubeCount; i++)
            _coords[i].moveBy(offset);
    }

    void TextBlock::reverseRotationAction(UserAction2D* action)
    {
        if (action->actionName == RotateLeft)
        {
            action->actionName = RotateRight;
        }
        else if (action->actionName == RotateRight)
        {
            action->actionName = RotateLeft;
        }
    }

    void TextBlock::setCoords()
    {
        //create random block with top-left board corner coordinates
        int random = (rand() % 6) + 1;

        switch (random)
        {
        case 1:
            _cubeCount = 1;
            _centerBlock = 0;
            _coords[0].x = 0; _coords[0].y = 0;
            break;
        case 2:
            _cubeCount = 4;
            _centerBlock = 1;
            _coords[0].x = 0; _coords[0].y = 0;
            _coords[1].x = 1; _coords[1].y = 0;
            _coords[2].x = 0; _coords[2].y = 1;
            _coords[3].x = 1; _coords[3].y = 1;
            break;
        case 3:
            _cubeCount = 4;
            _centerBlock = 2;
            _coords[0].x = 0; _coords[0].y = 0;
            _coords[1].x = 1; _coords[1].y = 0;
            _coords[2].x = 1; _coords[2].y = 1;
            _coords[3].x = 2; _coords[3].y = 1;
            break;
        case 4:
            _cubeCount = 4;
            _centerBlock = 2;
            _coords[0].x = 1; _coords[0].y = 0;
            _coords[1].x = 0; _coords[1].y = 1;
            _coords[2].x = 1; _coords[2].y = 1;
            _coords[3].x = 2; _coords[3].y = 1;
            break;
        case 5:
            _cubeCount = 4;
            _centerBlock = 1;
            _coords[0].x = 0; _coords[0].y = 0;
            _coords[1].x = 1; _coords[1].y = 0;
            _coords[2].x = 2; _coords[2].y = 0;
            _coords[3].x = 3; _coords[3].y = 0;
            break;
        case 6:
            _cubeCount = 4;
            _centerBlock = 2;
            _coords[0].x = 0; _coords[0].y = 0;
            _coords[1].x = 0; _coords[1].y = 1;
            _coords[2].x = 1; _coords[2].y = 1;
            _coords[3].x = 2; _coords[3].y = 1;
            break;
        }
    }
}
