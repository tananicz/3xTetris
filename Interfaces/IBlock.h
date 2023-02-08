#ifndef I_BLOCK_H
#define I_BLOCK_H

#include "IAction.h"
#include "IPosition.h"
#include "IBoard.h"

class IBlock
{
public:
	virtual ~IBlock() = 0 {};
	virtual void setInitialPosition(IPosition* position) = 0;
	virtual bool tryPush(IBoard* board) = 0;
	virtual bool tryAction(IBoard* board, IAction* action) = 0;
};

#endif // I_BLOCK_H
