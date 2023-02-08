#ifndef I_BOARD_H
#define I_BOARD_H

#include "IPosition.h"

class IBlock;

class IBoard
{
public:
	virtual ~IBoard() = 0 {};
	virtual IPosition* getStartingPosition() = 0;
	virtual bool canInsertBlock(IBlock* block) = 0;
	virtual void imprintBlock(IBlock* block) = 0;
	virtual int collectRows() = 0;
};

#endif // I_BOARD_H
