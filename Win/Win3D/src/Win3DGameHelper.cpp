#include "../headers/Win3DGameHelper.h"

#include "../headers/Win3DActionListener.h"
#include "../headers/Win3DBoard.h"
#include "../headers/Win3DBlock.h"
#include "../headers/Point3D.h"

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
		_graphicsHelper->beginDraw();
		_graphicsHelper->clear(D2D1::ColorF(D2D1::ColorF::White));
		_graphicsHelper->drawText(L"Welcome to 3D Tetris game!", 26, 30, 50, 50, D2D1::ColorF(D2D1::ColorF::Red));
		_graphicsHelper->drawText(L"S - start new game", 18, 20, 50, 300, D2D1::ColorF(D2D1::ColorF::Black));
		_graphicsHelper->drawText(L"Esc - exit game", 15, 20, 50, 330, D2D1::ColorF(D2D1::ColorF::Black));
		_graphicsHelper->endDraw();
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
		Win3DBlock* win3DBlock = static_cast<Win3DBlock*>(block);
		Win3DBoard* win3DBoard = static_cast<Win3DBoard*>(board);
		int width = win3DBoard->getWellSideSize();
		ColorEnum cubeColor;

		_graphicsHelper->setupPaintConfig(win3DBoard->getWellSideSize(), win3DBoard->getWellDepth());
		_graphicsHelper->beginDraw();
		_graphicsHelper->clear(D2D1::ColorF(D2D1::ColorF::Black));
		_graphicsHelper->draw3DWalls(win3DBoard->getWellDepth());

		Point3D origins[4];
		Point3D rightVect(1, 0, 0), leftVect(-1, 0, 0), upVect(0, -1, 0), downVect(0, 1, 0);

		for (int z = win3DBoard->getWellDepth() - 1; z >= 0; z--)
		{
			for (int i = 0; i < width / 2; i++)
			{
				origins[0].x = i; origins[0].y = i; origins[0].z = z;
				origins[1].x = width - 1 - i; origins[1].y = i; origins[1].z = z;
				origins[2].x = width - 1 - i; origins[2].y = width - 1 - i; origins[2].z = z;
				origins[3].x = i; origins[3].y = width - 1 - i; origins[3].z = z;

				for (size_t k = 0; k < 4; k++)
					tryDrawCube(win3DBlock, win3DBoard, origins[k]);

				Point3D cubeTL_H(origins[0]), cubeTL_V(origins[0]);
				Point3D cubeTR_H(origins[1]), cubeTR_V(origins[1]);
				Point3D cubeBR_H(origins[2]), cubeBR_V(origins[2]);
				Point3D cubeBL_H(origins[3]), cubeBL_V(origins[3]);

				while (cubeTL_H.x + 1 < width / 2)
				{
					cubeTL_H.moveBy(rightVect); 
					tryDrawCube(win3DBlock, win3DBoard, cubeTL_H);
					cubeTL_V.moveBy(downVect);
					tryDrawCube(win3DBlock, win3DBoard, cubeTL_V);

					cubeTR_H.moveBy(leftVect);
					tryDrawCube(win3DBlock, win3DBoard, cubeTR_H);
					cubeTR_V.moveBy(downVect);
					tryDrawCube(win3DBlock, win3DBoard, cubeTR_V);

					cubeBR_H.moveBy(leftVect);
					tryDrawCube(win3DBlock, win3DBoard, cubeBR_H);
					cubeBR_V.moveBy(upVect);
					tryDrawCube(win3DBlock, win3DBoard, cubeBR_V);

					cubeBL_H.moveBy(rightVect);
					tryDrawCube(win3DBlock, win3DBoard, cubeBL_H);
					cubeBL_V.moveBy(upVect);
					tryDrawCube(win3DBlock, win3DBoard, cubeBL_V);
				} 

				if (width % 2 != 0)
				{
					cubeTL_H.moveBy(rightVect);
					tryDrawCube(win3DBlock, win3DBoard, cubeTL_H);

					cubeTR_V.moveBy(downVect);
					tryDrawCube(win3DBlock, win3DBoard, cubeTR_V);

					cubeBR_H.moveBy(leftVect);
					tryDrawCube(win3DBlock, win3DBoard, cubeBR_H);

					cubeBL_V.moveBy(upVect);
					tryDrawCube(win3DBlock, win3DBoard, cubeBL_V);
				}
			}

			if (width % 2 != 0)
			{
				Point3D centerCube(width / 2, width / 2, z);
				tryDrawCube(win3DBlock, win3DBoard, centerCube);
			}
		}

		_graphicsHelper->endDraw();
	}

	void Win3DGameHelper::tryDrawCube(Win3DBlock* block, Win3DBoard* board, Point3D coords)
	{
		ColorEnum cubeColor;

		if (block->hasCoords(coords))
		{
			cubeColor = block->getColor();
		}
		else
		{
			cubeColor = board->getColorAtBoardPos(coords);
		}

		if (cubeColor != None)
			_graphicsHelper->drawCube(coords, cubeColor);
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
