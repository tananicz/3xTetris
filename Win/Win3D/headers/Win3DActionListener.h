#ifndef WIN_3D_ACTION_LISTENER_H
#define WIN_3D_ACTION_LISTENER_H

#include "../../../Interfaces/IActionListener.h"
#include "../../../Interfaces/IAction.h"
#include "../headers/Win3DActionEnum.h"
#include <Windows.h>

namespace Win3D
{
	class Win3DActionListener : public IActionListener
	{
	public:
		Win3DActionListener(HWND windowHandle);

		//IActionListener implementations
		IAction* listenForAction(bool waitForInput);

	private:
		HWND _windowHandle;
		IAction* createAction(Win3DActionEnum actionName);
	};
}

#endif
