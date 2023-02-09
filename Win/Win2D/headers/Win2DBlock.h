#ifndef WIN_2D_BLOCK_H
#define WIN_2D_BLOCK_H

#include "../../../Console2D/headers/TextBlock.h"

namespace Win2D
{
	class Win2DBlock : public TextBlock
	{
		public:
			bool tryAction(IBoard* board, IAction* action);
	};
}

#endif
