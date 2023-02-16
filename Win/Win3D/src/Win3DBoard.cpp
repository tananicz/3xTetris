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

		ColorEnum* baseBoardPtr = (ColorEnum*)_board;
		ColorEnum* boardLevelPtr = nullptr;
		ColorEnum* boardPtr = nullptr;
		int rowOffset = _WELL_SIDE_SIZE * _WELL_SIDE_SIZE;
		bool rowComplete;
		int j;

		for (int i = 0; i < _WELL_DEPTH; i++)
		{
			rowComplete = true;
			boardLevelPtr = baseBoardPtr + i * rowOffset;
			boardPtr = boardLevelPtr;

			for (j = 0; j < rowOffset; j++)
			{
				if (*(boardPtr + j) == None)
				{
					rowComplete = false;
					break;
				}
			}
			j--;

			if (rowComplete)
			{
				rowsCollected++;

				for (ColorEnum* tmpPtr = boardLevelPtr - 1; tmpPtr >= baseBoardPtr; tmpPtr--)
				{
					*(boardPtr + j) = *tmpPtr;
					j--;
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
}
