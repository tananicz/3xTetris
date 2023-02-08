#ifndef I_GAME_FACTORY_H
#define I_GAME_FACTORY_H

#include "IBlock.h"
#include "IBoard.h"
#include "IGameHelper.h"

class IGameFactory
{
public:
	virtual ~IGameFactory() = 0 {};
	virtual IGameHelper* getGameHelperSingleton() = 0;
	virtual IBoard* getBoard() = 0;
	virtual IBlock* getBlock() = 0;
};

#endif // I_GAME_FACTORY_H
