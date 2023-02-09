#ifndef WIN_2D_ACTION_H
#define WIN_2D_ACTION_H

#include "../../../Interfaces/IAction.h"
#include "../headers/Win2DActionEnum.h"

namespace Win2D
{
	struct Win2DAction : public IAction
	{
	public:
		Win2DActionEnum actionName;

		//IAction implementations
		bool isExitRequested();
		bool isStartRequested();
		bool isPauseRequested();
		bool isRefreshRequested();
	};
}

#endif
