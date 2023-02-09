#include "../headers/Win3DGameFactory.h"

namespace Win3D
{
	Win3DGameFactory::Win3DGameFactory()
	{
		_gameHelper = nullptr;
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
		return nullptr;
	}

	IBlock* Win3DGameFactory::getBlock()
	{
		return nullptr;
	}
}