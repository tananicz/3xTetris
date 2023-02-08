#ifndef I_ACTION_H
#define I_ACTION_H

struct IAction
{
public:
	virtual ~IAction() = 0 {};
	virtual bool isExitRequested() = 0;
	virtual bool isStartRequested() = 0;
	virtual bool isPauseRequested() = 0;
	virtual bool isRefreshRequested() = 0;
};

#endif // I_ACTION_H
