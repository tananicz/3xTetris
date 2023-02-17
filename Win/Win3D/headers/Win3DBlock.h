#ifndef WIN_3D_BLOCK_H
#define WIN_3D_BLOCK_H

#include "../../../Interfaces/IBlock.h"
#include "../headers/Point3D.h"
#include "../headers/Win3DBoard.h"
#include "../headers/Win3DAction.h"
#include "../../../Console2D/headers/ColorEnum.h"

using namespace Console2D;

namespace Win3D
{
	class Win3DBlock : public IBlock
	{
	public:
		Win3DBlock();
		ColorEnum getColor();
		int getBlockSize();
		Point3D getPositionAt(int i);
		bool hasCoords(Point3D point);

		//IBlock implementations
		void setInitialPosition(IPosition* position);
		bool tryPush(IBoard* board);
		bool tryAction(IBoard* board, IAction* action);

	private:
		int _cubeCount;
		ColorEnum _color;
		Point3D _coords[10];
		int _centerBlock;
		void setCoords();
		bool tryMove(Win3DBoard* board, Win3DAction* action);
		bool tryRotate(Win3DBoard* board, Win3DAction* action);
		void performRotation(Win3DAction* action);
		void reverseRotationAction(Win3DAction* action);
	};
}

#endif // WIN_3D_BLOCK_H

