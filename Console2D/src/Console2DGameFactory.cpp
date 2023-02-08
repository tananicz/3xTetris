#include "../headers/Console2DGameFactory.h"

#include "../headers/TextBoard.h"
#include "../headers/TextBlock.h"
#include "../headers/GameHelper.h"

namespace Console2D
{
	Console2DGameFactory::Console2DGameFactory()
	{
		this->_gameHelper = new GameHelper;
	}

	Console2DGameFactory::~Console2DGameFactory()
	{
		delete this->_gameHelper;
		this->_gameHelper = nullptr;
	}

	IGameHelper* Console2DGameFactory::getGameHelperSingleton()
	{
		return _gameHelper;
	}

	IBoard* Console2DGameFactory::getBoard()
	{
		return new TextBoard;
	}

	IBlock* Console2DGameFactory::getBlock()
	{
		return new TextBlock;
	}
}
