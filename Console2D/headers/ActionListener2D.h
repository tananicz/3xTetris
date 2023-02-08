#ifndef ACTION_LISTENER_2D_H
#define ACTION_LISTENER_2D_H

#include "../../Interfaces/IActionListener.h"

namespace Console2D
{
	class ActionListener2D : public IActionListener
	{
	public:
		IAction* listenForAction(bool waitForInput);
	};
}

#endif // ACTION_LISTENER_2D_H
