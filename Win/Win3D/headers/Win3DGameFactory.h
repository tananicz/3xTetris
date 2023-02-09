#ifndef WIN_3D_GAME_FACTORY_H
#define WIN_3D_GAME_FACTORY_H

#include "../../../Interfaces/IGameFactory.h"

namespace Win3D
{
	class Win3DGameFactory : public IGameFactory
	{
	public:
		Win3DGameFactory();
		~Win3DGameFactory();

		//IGameFactory implementations
		IGameHelper* getGameHelperSingleton();
		IBoard* getBoard();
		IBlock* getBlock();

	private:
		IGameHelper* _gameHelper;
	};
}

#endif
