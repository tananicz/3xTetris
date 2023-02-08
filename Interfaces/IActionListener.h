#ifndef I_ACTION_LISTENER_H
#define I_ACTION_LISTENER_H

#include "IAction.h"

class IActionListener
{
public:
	virtual ~IActionListener() = 0 {};
	virtual IAction* listenForAction(bool waitForInput) = 0;
};

#endif // I_ACTION_LISTENER_H
