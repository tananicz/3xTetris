#ifndef WIN_3D_BOARD_H
#define WIN_3D_BOARD_H

#include "../../../Interfaces/IBoard.h"
#include "../../../Console2D/headers/ColorEnum.h"
#include "../headers/Point3D.h"

using namespace Console2D;

namespace Win3D
{
	class Win3DBlock;

	class Win3DBoard : public IBoard
	{
	public:
		Win3DBoard();

		int getWellSideSize();
		int getWellDepth();
		bool isCollision(Win3DBlock* block);
		ColorEnum getColorAtBoardPos(Point3D point);

		//IBoard implementations
		IPosition* getStartingPosition();
		bool canInsertBlock(IBlock* block);
		void imprintBlock(IBlock* block);
		int collectRows();

	private:
		static const int _WELL_SIDE_SIZE = 7;
		static const int _WELL_DEPTH = 10;
		ColorEnum _board[_WELL_SIDE_SIZE][_WELL_SIDE_SIZE][_WELL_DEPTH];
	};
}

#endif
