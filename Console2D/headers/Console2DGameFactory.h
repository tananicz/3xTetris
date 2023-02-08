#ifndef CONSOLE_2D_GAME_FACTORY_H
#define CONSOLE_2D_GAME_FACTORY_H

#include "../../Interfaces/IGameFactory.h"

namespace Console2D
{
	class Console2DGameFactory : public IGameFactory
	{
	public:
		Console2DGameFactory();
		~Console2DGameFactory();

		//IGameFactory implementations
		IGameHelper* getGameHelperSingleton();
		IBoard* getBoard();
		IBlock* getBlock();

	private:
		IGameHelper* _gameHelper;
	};
}

#endif // CONSOLE_2D_GAME_FACTORY_H
