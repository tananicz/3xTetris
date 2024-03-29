#include "../headers/Win3DGameFactory.h"

#include "../headers/Win3DGameHelper.h"
#include "../headers/Win3DBoard.h"

namespace Win3D
{
	Win3DGameFactory::Win3DGameFactory()
	{
		_gameHelper = new Win3DGameHelper;
	}

	Win3DGameFactory::~Win3DGameFactory()
	{
		delete _gameHelper;
		_gameHelper = nullptr;
	}

	IGameHelper* Win3DGameFactory::getGameHelperSingleton()
	{
		return _gameHelper;
	}

	IBoard* Win3DGameFactory::getBoard()
	{
		return new Win3DBoard;
	}

	IBlock* Win3DGameFactory::getBlock()
	{
		return new Win3DBlock;
	}
}