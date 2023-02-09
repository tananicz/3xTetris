#include "../headers/Win2DAction.h"

namespace Win2D
{
	bool Win2DAction::isExitRequested()
	{
		if (actionName == ExitGame)
			return true;
		else
			return false;
	}

	bool Win2DAction::isStartRequested()
	{
		if (actionName == StartGame)
			return true;
		else
			return false;
	}

	bool Win2DAction::isPauseRequested()
	{
		return false;
	}

	bool Win2DAction::isRefreshRequested()
	{
		if (actionName == Refresh)
			return true;
		else
			return false;
	}
}
