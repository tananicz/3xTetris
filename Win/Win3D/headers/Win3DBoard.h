#ifndef WIN_3D_BOARD_H
#define WIN_3D_BOARD_H

#include "../../../Interfaces/IBoard.h"

namespace Win3D
{
	class Win3DBoard : public IBoard
	{
	public:
		Win3DBoard();
		~Win3DBoard();

		int getWellSideSize();
		int getWellDepth();

		//IBoard implementations
		IPosition* getStartingPosition();
		bool canInsertBlock(IBlock* block);
		void imprintBlock(IBlock* block);
		int collectRows();

	private:
		static const int _WELL_SIDE_SIZE = 12;
		static const int _WELL_DEPTH = 15;
	};
}

#endif
