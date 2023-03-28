#include "../headers/AbstractWinGameHelper.h"

#include <cmath>

namespace Win
{
	AbstractWinGameHelper::AbstractWinGameHelper()
	{
		_hInstance = 0;
		_windowHandle = 0;
		_actionListener = nullptr;
	}

	AbstractWinGameHelper::~AbstractWinGameHelper()
	{
		if (_actionListener != nullptr)
		{
			delete _actionListener;
			_actionListener = nullptr;
		}
	}

	bool AbstractWinGameHelper::initSystemSettings(HINSTANCE hInstance, int nCmdShow)
	{
		_hInstance = hInstance;

		//creating & registering window class
		WNDCLASS wc = { };
		wc.lpfnWndProc = WindowProc;
		wc.hInstance = hInstance;
		wc.lpszClassName = getWindowClassName();
		RegisterClass(&wc);

		//creating window
		_windowHandle = CreateWindowEx(0, getWindowClassName(), getWindowTitle(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 850, 750, NULL, NULL, hInstance, NULL);

		if (_windowHandle == NULL)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	IAction* AbstractWinGameHelper::getWelcomeScreenAction()
	{
		if (_actionListener != nullptr)
		{
			paintWelcomeScreen();
			IAction* action = nullptr;

			do
			{
				action = _actionListener->listenForAction(false);
				if (action != nullptr)
				{
					if (action->isStartRequested() || action->isExitRequested())
					{
						break;
					}
					else
					{
						if (action->isRefreshRequested())
							paintWelcomeScreen();

						delete action;
						action = nullptr;
					}
				}
			} while (1);

			return action;
		}
		return nullptr;
	}

	IAction* AbstractWinGameHelper::getGameOverScreenAction()
	{
		if (_actionListener != nullptr)
		{
			IAction* action = nullptr;
			paintGameOverScreen();

			do
			{
				action = _actionListener->listenForAction(false);
				if (action != nullptr)
				{
					if (action->isExitRequested())
					{
						break;
					}
					else
					{
						if (action->isRefreshRequested())
							paintGameOverScreen();

						delete action;
						action = nullptr;
					}
				}
			} while (1);

			return action;
		}
		return nullptr;
	}

	LRESULT CALLBACK AbstractWinGameHelper::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		//default window procedure for sent messages (i.e. messages directly sent by the system and omitting message queue)
		switch (uMsg)
		{
		case WM_CLOSE:
			/*
			A little explanation here - when user closes the window it is not WM_CLOSE sent to message queue, it's WM_NCLBUTTONDOWN instead. In fact PeekMessage inside
			Win2DActionListener::listenForAction(bool) never gets WM_CLOSE in such a way. So, the solution is that inside Win2DActionListener::listenForAction(bool) method
			we pass WM_NCLBUTTONDOWN to window procedure (via DispatchMessage).	This will trigger some system operations that will eventually call window procedure
			with WM_CLOSE message that we cope with here (it omits message queue, but one can still catch it inside window procedure). By intercepting such message here
			we call PostMessage that will force WM_CLOSE to be put inside message queue. We catch it inside Win2DActionListener::listenForAction(bool) - it will generate
			our IAction object with exit state set to true. That will allow our program to end in a proper way. Otherwise we'd have only window closed but still
			reside inside our custom message loop.
			*/
			PostMessage(hwnd, uMsg, wParam, lParam);
			return 0;
		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}

	IActionListener* AbstractWinGameHelper::getActionListener()
	{
		return _actionListener;
	}

	double AbstractWinGameHelper::updateInterval(int points)
	{
		int factor = points / 300;
		return 1 - (factor * 0.1);
	}

	wchar_t* AbstractWinGameHelper::intToWcharTPtr(int i)
	{
		if (i < 0)
		{
			return nullptr;
		}
		else if (i == 0)
		{
			wchar_t* result = new wchar_t[2];
			*result = (wchar_t)48;
			*(result + 1) = 0;

			return result;
		}
		else
		{
			int k = 0;
			while (i / (int)pow(10, k) != 0)
				k++;

			wchar_t* result = new wchar_t[k + 1];
			*(result + k) = 0;

			int tmp;
			int pow10;
			for (int j = k - 1; j >= 0; j--)
			{
				pow10 = (int)pow(10, j);
				tmp = i / pow10;
				i -= tmp * pow10;

				*(result + ((k - 1) - j)) = (wchar_t)(tmp + 48);
			}

			return result;
		}
	}
}