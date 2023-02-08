#include "../headers/TextBoard.h"

#include "../../Interfaces/IBlock.h"
#include "../headers/TextBlock.h"
#include "../headers/ColorEnum.h"
#include "../headers/Point.h"

using namespace std;

namespace Console2D
{
    TextBoard::TextBoard()
    {
        for (int i = 0; i < _WIDTH; i++)
            for (int j = 0; j < _HEIGHT; j++)
                _board[i][j] = None;
    }

    int TextBoard::getWidth()
    {
        return _WIDTH;
    }

    int TextBoard::getHeight()
    {
        return _HEIGHT;
    }

    ColorEnum TextBoard::getColorEnumAtPos(int x, int y)
    {
        return _board[x][y];
    }

    bool TextBoard::isCollision(TextBlock* textBlock)
    {
        for (int i = 0; i < textBlock->getBlockSize(); i++)
        {
            Point p = textBlock->getPositionAt(i);

            if ((p.x < 0) || (p.x > _WIDTH - 1))
                return true;
            if ((p.y < 0) || (p.y > _HEIGHT - 1))
                return true;

            if (_board[p.x][p.y] != None)
                return true;
        }
        return false;
    }

    IPosition* TextBoard::getStartingPosition()
    {
        Point* p = new Point;
        p->x = _WIDTH / 2;
        p->y = 0;
        return p;
    }

    bool TextBoard::canInsertBlock(IBlock* block)
    {
        return !isCollision(static_cast<TextBlock*>(block));
    }

    void TextBoard::imprintBlock(IBlock* block)
    {
        TextBlock* textBlock = static_cast<TextBlock*>(block);

        ColorEnum blockColor = textBlock->getColor();
        for (int i = 0; i < textBlock->getBlockSize(); i++)
        {
            Point cubeCoords = textBlock->getPositionAt(i);
            _board[cubeCoords.x][cubeCoords.y] = blockColor;
        }
    }

    int TextBoard::collectRows()
    {
        int result = 0;
        bool rowComplete;

        for (int row = 0; row < _HEIGHT; row++)
        {
            rowComplete = true;
            for (int col = 0; col < _WIDTH; col++)
            {
                if (_board[col][row] == None)
                {
                    rowComplete = false;
                    break;
                }
            }

            if (rowComplete)
            {
                result++;

                if (row == 0)
                {
                    for (int col = 0; col < _WIDTH; col++)
                        _board[col][row] = None;
                }
                else
                {
                    for (int i = 1; i <= row; i++)
                        for (int col = 0; col < _WIDTH; col++)
                            _board[col][row - (i - 1)] = _board[col][row - i];
                }
            }
        }

        return result;
    }

    bool TextBoard::isPositionInsideBlock(TextBlock* block, int x, int y)
    {
        Point p = { };
        for (int i = 0; i < block->getBlockSize(); i++)
        {
            p = block->getPositionAt(i);
            if ((p.x == x) && (p.y == y))
                return true;
        }
        return false;
    }
}
