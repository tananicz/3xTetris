#include "../headers/Win3DAction.h"

namespace Win3D
{
	bool Win3DAction::isExitRequested()
	{
		if (actionName == ExitGame)
			return true;
		else
			return false;
	}

	bool Win3DAction::isStartRequested()
	{
		if (actionName == StartGame)
			return true;
		else
			return false;
	}

	bool Win3DAction::isPauseRequested()
	{
		return false;
	}

	bool Win3DAction::isRefreshRequested()
	{
		if (actionName == Refresh)
			return true;
		else
			return false;
	}
}