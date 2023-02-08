#ifndef WIN_2D_ACTION_LISTENER_H
#define WIN_2D_ACTION_LISTENER_H

#include "../../Interfaces/IActionListener.h"
#include "../../Interfaces/IAction.h"
#include "../headers/Win2DActionEnum.h"
#include <Windows.h>

namespace Win2D
{
	class Win2DActionListener : public IActionListener
	{
	public:
		Win2DActionListener(HWND windowHandle);

		//IActionListener implementations
		IAction* listenForAction(bool waitForInput);

	private:
		HWND _windowHandle;
		IAction* createAction(Win2DActionEnum actionName);
	};
}

#endif
