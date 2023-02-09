#include "../headers/Win2DGameHelper.h"

#include "../headers/Win2DActionListener.h"
#include "../headers/Win2DBlock.h"
#include "../../../Console2D/headers/TextBoard.h"
#include <Windows.h>

namespace Win2D
{
	Win2DGameHelper::Win2DGameHelper()
	{
		_graphicsHelper = nullptr;
	}

	Win2DGameHelper::~Win2DGameHelper()
	{
		if (_graphicsHelper != nullptr)
		{
			delete _graphicsHelper;
			_graphicsHelper = nullptr;
		}
	}

	bool Win2DGameHelper::initSystemSettings(HINSTANCE hInstance, int nCmdShow)
	{
		if (!Win::AbstractWinGameHelper::initSystemSettings(hInstance, nCmdShow))
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
				if (txtBoard->isPositionInsideBlock(txtBlock, (int)x, (int)y))
				{
					color = txtBlock->getColor();
				}
				else
				{
					color = txtBoard->getColorEnumAtPos((int)x, (int)y);
				}

				_graphicsHelper->drawSquareColorEnum(((int)x * _cubeWidth) + _screenOffsetX + _cubeWidth, ((int)y * _cubeWidth) + _screenOffsetY, _cubeWidth, color, true);
			}
		}

		_graphicsHelper->endDraw();
	}

	void Win2DGameHelper::paintBorders(int boardWidth, int boardHeight, D2D1::ColorF color)
	{
		_graphicsHelper->drawRectangleCustom(_screenOffsetX, _screenOffsetY, _cubeWidth, (boardHeight + 1) * _cubeWidth, color, false, color);
		_graphicsHelper->drawRectangleCustom(_screenOffsetX + (1 + boardWidth) * _cubeWidth, _screenOffsetY, _cubeWidth, (boardHeight + 1) * _cubeWidth, color, false, color);
		_graphicsHelper->drawRectangleCustom(_screenOffsetX + _cubeWidth, _screenOffsetY + boardHeight * _cubeWidth, boardWidth * _cubeWidth, _cubeWidth, color, false, color);
	}

	const wchar_t* Win2DGameHelper::getWindowClassName()
	{
		return L"2D Tetris Window Class";
	}

	const wchar_t* Win2DGameHelper::getWindowTitle()
	{
		return L"2D Tetris";
	}
}
