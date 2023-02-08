#include "../headers/Win2DGameHelper.h"

#include "../headers/Win2DActionListener.h"
#include "../headers/Win2DBlock.h"
#include "../../Console2D/headers/TextBoard.h"
#include <Windows.h>
#include <d2d1.h>
#include <dwrite.h>
#include <cmath>

namespace Win2D
{
	Win2DGameHelper::Win2DGameHelper()
	{
		_hInstance = 0;
		_windowHandle = 0;
		_graphicsHelper = nullptr;
		_actionListener = nullptr;
	}

	Win2DGameHelper::~Win2DGameHelper()
	{
		if (_actionListener != nullptr)
		{
			delete _actionListener;
			_actionListener = nullptr;
		}

		if (_graphicsHelper != nullptr)
		{
			delete _graphicsHelper;
			_graphicsHelper = nullptr;
		}
	}

	bool Win2DGameHelper::initSystemSettings(HINSTANCE hInstance, int nCmdShow)
	{
		_hInstance = hInstance;

		//creating & registering window class
		const wchar_t WINDOW_CLASS_NAME[] = L"2D Tetris Window Class";

		WNDCLASS wc = { };
		wc.lpfnWndProc = WindowProc;
		wc.hInstance = hInstance;
		wc.lpszClassName = WINDOW_CLASS_NAME;
		RegisterClass(&wc);

		//creating window
		_windowHandle = CreateWindowEx(0, WINDOW_CLASS_NAME, L"2D Tetris", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

		if (_windowHandle == NULL)
		{
			return false;
		}
		else
		{
			_graphicsHelper = new Win2DGraphicsHelper(_windowHandle);

			if (_graphicsHelper->initializeFactories())
			{
				_actionListener = new Win2DActionListener(_windowHandle);
				ShowWindow(_windowHandle, nCmdShow);

				return true;
			}
			else
			{
				delete _graphicsHelper;
				_graphicsHelper = nullptr;

				return false;
			}
		}
	}

	IActionListener* Win2DGameHelper::getActionListener()
	{
		return _actionListener;
	}

	IAction* Win2DGameHelper::getWelcomeScreenAction()
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
			} 
			while (1);

			return action;
		}
		return nullptr;
	}

	void Win2DGameHelper::paintWelcomeScreen()
	{
		_graphicsHelper->beginDraw();
		_graphicsHelper->clear(D2D1::ColorF(D2D1::ColorF::White));
		_graphicsHelper->drawText(L"Welcome to Tetris game!", 23, 30, 50, 50, D2D1::ColorF(D2D1::ColorF::Red));
		_graphicsHelper->drawText(L"Use left and right arrow keys to move the block horizontally", 60, 13, 50, 150, D2D1::ColorF(D2D1::ColorF::Black));
		_graphicsHelper->drawText(L"Use down arrow key to push the block downwards", 46, 13, 50, 175, D2D1::ColorF(D2D1::ColorF::Black));
		_graphicsHelper->drawText(L"Use A and Z keys to rotate the block", 36, 13, 50, 200, D2D1::ColorF(D2D1::ColorF::Black));
		_graphicsHelper->drawText(L"S - start new game", 18, 20, 50, 300, D2D1::ColorF(D2D1::ColorF::Black));
		_graphicsHelper->drawText(L"Esc - exit game", 15, 20, 50, 330, D2D1::ColorF(D2D1::ColorF::Black));
		_graphicsHelper->endDraw();
	}

	IAction* Win2DGameHelper::getGameOverScreenAction()
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

	void Win2DGameHelper::paintGameOverScreen()
	{
		_graphicsHelper->beginDraw();
		_graphicsHelper->clear(D2D1::ColorF(D2D1::ColorF::White));
		_graphicsHelper->drawText(L"GAME OVER!", 10, 30, 50, 50, D2D1::ColorF(D2D1::ColorF::Red));
		_graphicsHelper->drawText(L"Esc - exit", 10, 13, 50, 150, D2D1::ColorF(D2D1::ColorF::Black));
		_graphicsHelper->endDraw();
	}

	void Win2DGameHelper::drawBoard(IBoard* board, IBlock* block, int points)
	{
		TextBoard* txtBoard = static_cast<TextBoard*>(board);
		Win2DBlock* txtBlock = static_cast<Win2DBlock*>(block);
		ColorEnum color = None;

		_graphicsHelper->beginDraw();
		_graphicsHelper->clear(D2D1::ColorF(D2D1::ColorF::Black));
		paintBorders(txtBoard->getWidth(), txtBoard->getHeight(), D2D1::ColorF(D2D1::ColorF::LightGray));

		wchar_t* pointsWcharPtr = intToWcharTPtr(points);
		if (pointsWcharPtr != nullptr)
		{
			_graphicsHelper->drawText(L"Points:", 7, 30, _screenOffsetX + (txtBoard->getWidth() + 2) * _cubeWidth + 50, 100, D2D1::ColorF(D2D1::ColorF::Red));
			_graphicsHelper->drawText(pointsWcharPtr, wcslen(pointsWcharPtr), 30, _screenOffsetX + (txtBoard->getWidth() + 2) * _cubeWidth + 50, 140, D2D1::ColorF(D2D1::ColorF::Red));
			delete[] pointsWcharPtr;
		}

		for (size_t y = 0; y < txtBoard->getHeight(); y++)
		{
			for (size_t x = 0; x < txtBoard->getWidth(); x++)
			{
				if (txtBoard->isPositionInsideBlock(txtBlock, (int) x, (int) y))
				{
					color = txtBlock->getColor();
				}
				else
				{
					color = txtBoard->getColorEnumAtPos((int) x, (int) y);
				}

				_graphicsHelper->drawSquareColorEnum(((int) x * _cubeWidth) + _screenOffsetX + _cubeWidth, ((int) y * _cubeWidth) + _screenOffsetY, _cubeWidth, color, true);
			}
		}

		_graphicsHelper->endDraw();
	}

	void Win2DGameHelper::paintBorders(int boardWidth, int boardHeight, D2D1::ColorF color)
	{
		_graphicsHelper->drawRectangleCustom(_screenOffsetX, _screenOffsetY, _cubeWidth, (boardHeight + 1) * _cubeWidth, color, false);
		_graphicsHelper->drawRectangleCustom(_screenOffsetX + (1 + boardWidth) * _cubeWidth, _screenOffsetY, _cubeWidth, (boardHeight + 1) * _cubeWidth, color, false);
		_graphicsHelper->drawRectangleCustom(_screenOffsetX + _cubeWidth, _screenOffsetY + boardHeight * _cubeWidth, boardWidth * _cubeWidth, _cubeWidth, color, false);
	}

	double Win2DGameHelper::updateInterval(int points)
	{
		int factor = points / 300;
		return 1 - (factor * 0.1);
	}

	wchar_t* Win2DGameHelper::intToWcharTPtr(int i)
	{
		if (i < 0)
		{
			return nullptr;
		}
		else if (i == 0)
		{
			wchar_t* result = new wchar_t[2];
			*result = (wchar_t) 48;
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

	LRESULT CALLBACK Win2DGameHelper::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
}
