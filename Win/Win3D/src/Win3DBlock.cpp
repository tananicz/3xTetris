#include "../headers/Win3DBlock.h"

#include "../headers/Win3DAction.h"
#include "../headers/Win3DBoard.h"
#include <stdlib.h>

namespace Win3D
{
    Win3DBlock::Win3DBlock()
    {
        for (size_t i = 0; i < 10; i++)
            _coords[i] = Point3D();

        _color = (ColorEnum)(rand() % 5 + 1);
        setCoords();
    }

    ColorEnum Win3DBlock::getColor()
    {
        return _color;
    }

    int Win3DBlock::getBlockSize()
    {
        return _cubeCount;
    }

    Point3D Win3DBlock::getPositionAt(int i)
    {
        return _coords[i];
    }

    bool Win3DBlock::hasCoords(Point3D point)
    {
        for (int i = 0; i < _cubeCount; i++)
        {
            if (_coords[i].x == point.x && _coords[i].y == point.y && _coords[i].z == point.z)
                return true;
        }
        return false;
    }

    void Win3DBlock::setInitialPosition(IPosition* position)
    {
        Point3D* pos3D = static_cast<Point3D*>(position);

        int minX, maxX, minY, maxY;
        minX = maxX = _coords[0].x;
        minY = maxY = _coords[0].y;

        for (int i = 1; i < _cubeCount; i++)
        {
            if (_coords[i].x < minX)
                minX = _coords[i].x;
            if (_coords[i].x > maxX)
                maxX = _coords[i].x;

            if (_coords[i].y < minY)
                minY = _coords[i].y;
            if (_coords[i].y > maxY)
                maxY = _coords[i].y;
        }

        int blockWidth = maxX - minX + 1;
        int blockHeight = maxY - minY + 1;

        Point3D offset;
        offset.x = pos3D->x - blockWidth / 2;
        offset.y = pos3D->y - blockHeight / 2;
        offset.z = 0;

        for (int i = 0; i < _cubeCount; i++)
            _coords[i].moveBy(offset);
    }

    bool Win3DBlock::tryPush(IBoard* board)
    {
        Win3DAction userAction;
        userAction.actionName = PushDown;
        return tryAction(board, &userAction);
    }

    bool Win3DBlock::tryAction(IBoard* board, IAction* action)
    {
        Win3DBoard* board3D = static_cast<Win3DBoard*>(board);
        Win3DAction* userAction = static_cast<Win3DAction*>(action);

        if (userAction->actionName == MoveDown || userAction->actionName == MoveUp || userAction->actionName == MoveLeft || userAction->actionName == MoveRight || userAction->actionName == PushDown)
        {
            return tryMove(board3D, userAction);
        }
        else if (userAction->actionName == RotateXCCW || userAction->actionName == RotateXCW || userAction->actionName == RotateYCCW || userAction->actionName == RotateYCW || userAction->actionName == RotateZCCW || userAction->actionName == RotateZCW)
        {
            return tryRotate(board3D, userAction);
        }
        else
            return false;
    }

    bool Win3DBlock::tryMove(Win3DBoard* board, Win3DAction* action)
    {
        Point3D offset;

        switch (action->actionName)
        {
        case MoveRight:
            offset.x = 1;
            offset.y = 0;
            offset.z = 0;
            break;
        case MoveLeft:
            offset.x = -1;
            offset.y = 0;
            offset.z = 0;
            break;
        case MoveDown:
            offset.x = 0;
            offset.y = 1;
            offset.z = 0;
            break;
        case MoveUp:
            offset.x = 0;
            offset.y = -1;
            offset.z = 0;
            break;
        case PushDown:
            offset.x = 0;
            offset.y = 0;
            offset.z = 1;
            break;
        default:
            offset.x = 0;
            offset.y = 0;
            offset.z = 0;
            break;
        }

        for (int i = 0; i < _cubeCount; i++)
            _coords[i].moveBy(offset);

        if (board->isCollision(this))
        {
            offset.x = -1 * offset.x;
            offset.y = -1 * offset.y;
            offset.z = -1 * offset.z;

            for (int i = 0; i < _cubeCount; i++)
                _coords[i].moveBy(offset);

            return false;
        }

        return true;
    }

    bool Win3DBlock::tryRotate(Win3DBoard* board, Win3DAction* action)
    {
        if (action->actionName == RotateXCW || action->actionName == RotateXCCW || action->actionName == RotateYCW || action->actionName == RotateYCCW || action->actionName == RotateZCW || action->actionName == RotateZCCW)
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

    void Win3DBlock::performRotation(Win3DAction* action)
    {
        Point3D offset = _coords[_centerBlock];

        offset.x = -1 * offset.x;
        offset.y = -1 * offset.y;
        offset.z = -1 * offset.z;

        for (int i = 0; i < _cubeCount; i++)
            _coords[i].moveBy(offset);

        for (int i = 0; i < _cubeCount; i++)
        {
            int tmp;

            if (action->actionName == RotateXCCW)
            {
                tmp = _coords[i].y;
                _coords[i].y = _coords[i].z;
                _coords[i].z = -1 * tmp;
            }
            else if (action->actionName == RotateXCW)
            {
                tmp = _coords[i].y;
                _coords[i].y = -1 * _coords[i].z;
                _coords[i].z = tmp;
            }
            if (action->actionName == RotateYCCW)
            {
                tmp = _coords[i].x;
                _coords[i].x = -1 * _coords[i].z;
                _coords[i].z = tmp;
            }
            else if (action->actionName == RotateYCW)
            {
                tmp = _coords[i].x;
                _coords[i].x = _coords[i].z;
                _coords[i].z = -1 * tmp;
            }
            if (action->actionName == RotateZCCW)
            {
                tmp = _coords[i].x;
                _coords[i].x = -1 * _coords[i].y;
                _coords[i].y = tmp;
            }
            else if (action->actionName == RotateZCW)
            {
                tmp = _coords[i].x;
                _coords[i].x = _coords[i].y;
                _coords[i].y = -1 * tmp;
            }
        }

        offset.x = -1 * offset.x;
        offset.y = -1 * offset.y;
        offset.z = -1 * offset.z;

        for (int i = 0; i < _cubeCount; i++)
            _coords[i].moveBy(offset);
    }

    void Win3DBlock::reverseRotationAction(Win3DAction* action)
    {
        if (action->actionName == RotateXCW)
        {
            action->actionName = RotateXCCW;
        }
        else if (action->actionName == RotateXCCW)
        {
            action->actionName = RotateXCW;
        }
        else if (action->actionName == RotateYCW)
        {
            action->actionName = RotateYCCW;
        }
        else if (action->actionName == RotateYCCW)
        {
            action->actionName = RotateYCW;
        }
        else if (action->actionName == RotateZCW)
        {
            action->actionName = RotateZCCW;
        }
        else if (action->actionName == RotateZCCW)
        {
            action->actionName = RotateZCW;
        }
    }

	void Win3DBlock::setCoords()
	{
        //create random block on top of well placed top-left
        int random = (rand() % 6) + 1;

        switch (random)
        {
        case 1:
            _cubeCount = 1;
            _centerBlock = 0;
            _coords[0].x = 0; _coords[0].y = 0; _coords[0].z = 0;
            break;
        case 2:
            _cubeCount = 8;
            _centerBlock = 7;
            _coords[0].x = 0; _coords[0].y = 0; _coords[0].z = 0;
            _coords[1].x = 1; _coords[1].y = 0; _coords[1].z = 0;
            _coords[2].x = 0; _coords[2].y = 1; _coords[2].z = 0;
            _coords[3].x = 1; _coords[3].y = 1; _coords[3].z = 0;
            _coords[4].x = 0; _coords[4].y = 0; _coords[4].z = 1;
            _coords[5].x = 1; _coords[5].y = 0; _coords[5].z = 1;
            _coords[6].x = 0; _coords[6].y = 1; _coords[6].z = 1;
            _coords[7].x = 1; _coords[7].y = 1; _coords[7].z = 1;
            break;
        case 3:
            _cubeCount = 4;
            _centerBlock = 2;
            _coords[0].x = 0; _coords[0].y = 0; _coords[0].z = 0;
            _coords[1].x = 1; _coords[1].y = 0; _coords[1].z = 0;
            _coords[2].x = 2; _coords[2].y = 0; _coords[2].z = 0;
            _coords[3].x = 3; _coords[3].y = 0; _coords[3].z = 0;
            break;
        case 4:
            _cubeCount = 4;
            _centerBlock = 1;
            _coords[0].x = 0; _coords[0].y = 0; _coords[0].z = 0;
            _coords[1].x = 1; _coords[1].y = 0; _coords[1].z = 0;
            _coords[2].x = 2; _coords[2].y = 0; _coords[2].z = 0;
            _coords[3].x = 0; _coords[3].y = 1; _coords[3].z = 0;
            break;
        case 5:
            _cubeCount = 4;
            _centerBlock = 2;
            _coords[0].x = 0; _coords[0].y = 0; _coords[0].z = 0;
            _coords[1].x = 1; _coords[1].y = 0; _coords[1].z = 0;
            _coords[2].x = 1; _coords[2].y = 1; _coords[2].z = 0;
            _coords[3].x = 2; _coords[3].y = 1; _coords[3].z = 0;
            break;
        case 6:
            _cubeCount = 10;
            _centerBlock = 5;
            _coords[0].x = 1; _coords[0].y = 1; _coords[0].z = 0;
            _coords[1].x = 0; _coords[1].y = 0; _coords[1].z = 1;
            _coords[2].x = 1; _coords[2].y = 0; _coords[2].z = 1;
            _coords[3].x = 2; _coords[3].y = 0; _coords[3].z = 1;
            _coords[4].x = 0; _coords[4].y = 1; _coords[4].z = 1;
            _coords[5].x = 1; _coords[5].y = 1; _coords[5].z = 1;
            _coords[6].x = 2; _coords[6].y = 1; _coords[6].z = 1;
            _coords[7].x = 0; _coords[7].y = 2; _coords[7].z = 1;
            _coords[8].x = 1; _coords[8].y = 2; _coords[8].z = 1;
            _coords[9].x = 2; _coords[9].y = 2; _coords[9].z = 1;
            break;
        }
	}
}
