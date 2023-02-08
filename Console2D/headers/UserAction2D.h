#ifndef USER_ACTION_2D_H
#define USER_ACTION_2D_H

#include "../../Interfaces/IAction.h"
#include "ActionEnum.h"

using namespace Console2D;

namespace Console2D
{
	struct UserAction2D : public IAction
	{
	public:
		ActionEnum actionName = NoAction;
		bool isExitRequested();
		bool isStartRequested();
		bool isPauseRequested();
		bool isRefreshRequested();
	};
}

#endif // USER_ACTION_2D_H
