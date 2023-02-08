#include "../headers/Win2DGameFactory.h"

#include "../headers/Win2DGameHelper.h"
#include "../headers/Win2DBlock.h"
#include "../../Console2D/headers/TextBoard.h"

namespace Win2D
{
	Win2DGameFactory::Win2DGameFactory()
	{
		_gameHelper = new Win2DGameHelper;
	}

	Win2DGameFactory::~Win2DGameFactory()
	{
		delete _gameHelper;
		_gameHelper = nullptr;
	}

	IGameHelper* Win2DGameFactory::getGameHelperSingleton()
	{
		return _gameHelper;
	}

	IBoard* Win2DGameFactory::getBoard()
	{
		return new Console2D::TextBoard;
	}

	IBlock* Win2DGameFactory::getBlock()
	{
		return new Win2DBlock;
	}
}
