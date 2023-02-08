#ifndef TEXT_BLOCK_H
#define TEXT_BLOCK_H

#include "../../Interfaces/IBlock.h"
#include "../../Interfaces/IAction.h"
#include "ColorEnum.h"
#include "Point.h"
#include "UserAction2D.h"

using namespace Console2D;

namespace Console2D
{
	class TextBoard;

	class TextBlock : public IBlock
	{
	public:
		TextBlock();
		ColorEnum getColor();
		int getBlockSize();
		Point getPositionAt(int i);

		//IBlock implementations
		void setInitialPosition(IPosition* position);
		bool tryPush(IBoard* board);
		bool tryAction(IBoard* board, IAction* action);

	private:
		ColorEnum _color;
		int _cubeCount;
		Point _coords[4];
		int _centerBlock;
		void setCoords();
		bool tryMove(TextBoard* board, UserAction2D* action);
		bool tryRotate(TextBoard* board, UserAction2D* action);
		void performRotation(UserAction2D* action);
		void reverseRotationAction(UserAction2D* action);
	};
}

#endif // TEXT_BLOCK_H
