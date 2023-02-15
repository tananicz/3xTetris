#include "../headers/Win3DBoard.h"

#include "../headers/Point3D.h"

namespace Win3D
{
	Win3DBoard::Win3DBoard()
	{
		for (size_t z = 0; z < _WELL_DEPTH; z++)
		{
			for (size_t x = 0; x < _WELL_SIDE_SIZE; x++)
			{
				for (size_t y = 0; x < _WELL_SIDE_SIZE; y++)
				{
					_board[x][y][z] = None;
				}
			}
		}
	}

	IPosition* Win3DBoard::getStartingPosition()
	{
		Point3D returnVal;

		returnVal.x = _WELL_SIDE_SIZE / 2;
		returnVal.y = _WELL_SIDE_SIZE / 2;
		returnVal.z = 0;

		return &returnVal;
	}

	bool Win3DBoard::canInsertBlock(IBlock* block)
	{
		return false;
	}

	void Win3DBoard::imprintBlock(IBlock* block)
	{

	}

	int Win3DBoard::collectRows()
	{
		return 0;
	}

	int Win3DBoard::getWellSideSize()
	{
		return _WELL_SIDE_SIZE;
	}

	int Win3DBoard::getWellDepth()
	{
		return _WELL_DEPTH;
	}
}
