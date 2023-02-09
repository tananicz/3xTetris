#ifndef WIN_3D_ACTION_H
#define WIN_3D_ACTION_H

#include "../../../Interfaces/IAction.h"
#include "../headers/Win3DActionEnum.h"

namespace Win3D
{
	struct Win3DAction : public IAction
	{
	public:
		Win3DActionEnum actionName;

		//IAction implementations
		bool isExitRequested();
		bool isStartRequested();
		bool isPauseRequested();
		bool isRefreshRequested();
	};
}

#endif
