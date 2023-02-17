#include "../headers/Win3DBoard.h"

#include "../headers/Point3D.h"
#include "../headers/Win3DBlock.h"

namespace Win3D
{
	Win3DBoard::Win3DBoard()
	{
		for (size_t z = 0; z < _WELL_DEPTH; z++)
		{
			for (size_t x = 0; x < _WELL_SIDE_SIZE; x++)
			{
				for (size_t y = 0; y < _WELL_SIDE_SIZE; y++)
				{
					_board[x][y][z] = None;
				}
			}
		}
	}

	IPosition* Win3DBoard::getStartingPosition()
	{
		Point3D* returnVal = new Point3D;

		returnVal->x = _WELL_SIDE_SIZE / 2;
		returnVal->y = _WELL_SIDE_SIZE / 2;
		returnVal->z = 0;

		return returnVal;
	}

	bool Win3DBoard::canInsertBlock(IBlock* block)
	{
		return !isCollision(static_cast<Win3DBlock*>(block));
	}

	void Win3DBoard::imprintBlock(IBlock* block)
	{
		Win3DBlock* block3D = static_cast<Win3DBlock*>(block);
		ColorEnum blockColor = block3D->getColor();

		for (int i = 0; i < block3D->getBlockSize(); i++)
		{
			Point3D cubeCoords = block3D->getPositionAt(i);
			_board[cubeCoords.x][cubeCoords.y][cubeCoords.z] = blockColor;
		}
	}

	int Win3DBoard::collectRows()
	{
		int rowsCollected = 0;
		bool rowComplete;

		for (size_t z = 0; z < _WELL_DEPTH; z++)
		{
			rowComplete = true;

			for (size_t x = 0; x < _WELL_SIDE_SIZE; x++)
				for (size_t y = 0; y < _WELL_SIDE_SIZE; y++)
					if (_board[x][y][z] == None)
					{
						rowComplete = false;
					}

			if (rowComplete)
			{
				rowsCollected++;

				if (z == 0)
				{
					for (size_t x = 0; x < _WELL_SIDE_SIZE; x++)
						for (size_t y = 0; y < _WELL_SIDE_SIZE; y++)
							_board[x][y][z] == None;
				}
				else
				{
					for (int i = 1; i <= z; i++)
					{
						for (size_t x = 0; x < _WELL_SIDE_SIZE; x++)
							for (size_t y = 0; y < _WELL_SIDE_SIZE; y++)
							{
								_board[x][y][z - i + 1] = _board[x][y][z - i];
							}
					}
				}
			}
		}

		return rowsCollected;
	}

	int Win3DBoard::getWellSideSize()
	{
		return _WELL_SIDE_SIZE;
	}

	int Win3DBoard::getWellDepth()
	{
		return _WELL_DEPTH;
	}

	bool Win3DBoard::isCollision(Win3DBlock* block)
	{
		for (int i = 0; i < block->getBlockSize(); i++)
		{
			Point3D p = block->getPositionAt(i);

			if ((p.x < 0) || (p.x > _WELL_SIDE_SIZE - 1))
				return true;
			if ((p.y < 0) || (p.y > _WELL_SIDE_SIZE - 1))
				return true;
			if ((p.z < 0) || (p.z > _WELL_DEPTH - 1))
				return true;

			if (_board[p.x][p.y][p.z] != None)
				return true;
		}
		return false;
	}

	ColorEnum Win3DBoard::getColorAtBoardPos(Point3D point)
	{
		return _board[point.x][point.y][point.z];
	}
}
