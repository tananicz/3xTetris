#include "../headers/Win3DGameHelper.h"

#include "../headers/Win3DActionListener.h"
#include "../headers/Win3DBoard.h"

namespace Win3D
{
	Win3DGameHelper::Win3DGameHelper()
	{
		_graphicsHelper = nullptr;
	}

	Win3DGameHelper::~Win3DGameHelper()
	{
		if (_graphicsHelper != nullptr)
		{
			delete _graphicsHelper;
			_graphicsHelper = nullptr;
		}
	}

	bool Win3DGameHelper::initSystemSettings(HINSTANCE hInstance, int nCmdShow)
	{
		if (!Win::AbstractWinGameHelper::initSystemSettings(hInstance, nCmdShow))
		{
			return false;
		}
		else
		{
			_graphicsHelper = new Win3DGraphicsHelper(_windowHandle);

			if (_graphicsHelper->initializeFactories() && _graphicsHelper->initializeDeviceIndependentResources())
			{
				_actionListener = new Win3DActionListener(_windowHandle);
				ShowWindow(_windowHandle, nCmdShow);

				return true;
			}
			else
			{
				//if initializeFactories() succeeds and initializeDeviceIndependentResources() fails, all the factories are released in Win3DGraphicsHelper destructor

				delete _graphicsHelper;
				_graphicsHelper = nullptr;

				return false;
			}
		}
	}

	void Win3DGameHelper::paintWelcomeScreen()
	{
		_graphicsHelper->setupPaintConfig(7, 12);

		_graphicsHelper->beginDraw();
		_graphicsHelper->clear(D2D1::ColorF(D2D1::ColorF::Black));
		_graphicsHelper->draw3DWalls(12);
		_graphicsHelper->drawCube(1, 6, 1, Red);
		_graphicsHelper->drawCube(1, 5, 1, Red);
		_graphicsHelper->drawCube(1, 6, 0, Red);
		_graphicsHelper->endDraw();

		/*_graphicsHelper->beginDraw();
		_graphicsHelper->clear(D2D1::ColorF(D2D1::ColorF::White));
		_graphicsHelper->drawText(L"Welcome to 3D Tetris game!", 26, 30, 50, 50, D2D1::ColorF(D2D1::ColorF::Red));
		_graphicsHelper->drawText(L"S - start new game", 18, 20, 50, 300, D2D1::ColorF(D2D1::ColorF::Black));
		_graphicsHelper->drawText(L"Esc - exit game", 15, 20, 50, 330, D2D1::ColorF(D2D1::ColorF::Black));
		_graphicsHelper->endDraw();*/
	}

	void Win3DGameHelper::paintGameOverScreen()
	{
		_graphicsHelper->beginDraw();
		_graphicsHelper->clear(D2D1::ColorF(D2D1::ColorF::White));
		_graphicsHelper->drawText(L"GAME OVER!", 10, 30, 50, 50, D2D1::ColorF(D2D1::ColorF::Red));
		_graphicsHelper->drawText(L"Esc - exit", 10, 13, 50, 150, D2D1::ColorF(D2D1::ColorF::Black));
		_graphicsHelper->endDraw();
	}

	void Win3DGameHelper::drawBoard(IBoard* board, IBlock* block, int points)
	{
		Win3DBoard* win3DBoard = static_cast<Win3DBoard*>(board);
		_graphicsHelper->setupPaintConfig(win3DBoard->getWellSideSize(), win3DBoard->getWellDepth());

		_graphicsHelper->beginDraw();
		_graphicsHelper->clear(D2D1::ColorF(D2D1::ColorF::Black));
		_graphicsHelper->draw3DWalls(win3DBoard->getWellDepth());
		_graphicsHelper->endDraw();
	}

	const wchar_t* Win3DGameHelper::getWindowClassName()
	{
		return L"3D Tetris Window Class";
	}

	const wchar_t* Win3DGameHelper::getWindowTitle()
	{
		return L"3D Tetris";
	}
}
