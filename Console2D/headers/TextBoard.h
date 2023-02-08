#ifndef TEXT_BOARD_H
#define TEXT_BOARD_H

#include "../../Interfaces/IBoard.h"
#include "ColorEnum.h"
#include "TextBlock.h"

namespace Console2D
{
    class TextBoard : public IBoard
    {
    public:
        TextBoard();
        bool isCollision(TextBlock* block);
        int getWidth();
        int getHeight();
        bool isPositionInsideBlock(TextBlock* block, int x, int y);
        ColorEnum getColorEnumAtPos(int x, int y);

        //IBoard implementations
        IPosition* getStartingPosition();
        bool canInsertBlock(IBlock* block);
        void imprintBlock(IBlock* block);
        int collectRows();

    protected:

    private:
        static const int _WIDTH = 20;
        static const int _HEIGHT = 25;
        ColorEnum _board[_WIDTH][_HEIGHT];
    };
}

#endif // TEXT_BOARD_H