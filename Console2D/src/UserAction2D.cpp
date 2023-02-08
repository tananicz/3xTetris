#include "../headers/UserAction2D.h"

namespace Console2D
{
	bool UserAction2D::isExitRequested()
	{
		if (actionName == ExitGame)
			return true;
		else
			return false;
	}

	bool UserAction2D::isStartRequested()
	{
		if (actionName == StartGame)
			return true;
		else
			return false;
	}

	bool UserAction2D::isPauseRequested()
	{
		return false;
	}

	bool UserAction2D::isRefreshRequested()
	{
		return false;
	}
}
