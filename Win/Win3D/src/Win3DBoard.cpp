#include "../headers/Win3DBoard.h"

namespace Win3D
{
	Win3DBoard::Win3DBoard()
	{
		//TODO
	}

	Win3DBoard::~Win3DBoard()
	{
		//TODO
	}

	IPosition* Win3DBoard::getStartingPosition()
	{
		return nullptr;
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
