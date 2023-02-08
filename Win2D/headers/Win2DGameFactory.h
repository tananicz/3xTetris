#ifndef WIN_2D_GAME_FACTORY_H
#define WIN_2D_GAME_FACTORY_H

#include "../../Interfaces/IGameFactory.h"

namespace Win2D
{
	class Win2DGameFactory : public IGameFactory
	{
	public:
		Win2DGameFactory();
		~Win2DGameFactory();

		//IGameFactory implementations
		IGameHelper* getGameHelperSingleton();
		IBoard* getBoard();
		IBlock* getBlock();

	private:
		IGameHelper* _gameHelper;
	};
}

#endif
