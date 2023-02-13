#include "../headers/Win2DGraphicsHelper.h"

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

namespace Win2D
{
	Win2DGraphicsHelper::Win2DGraphicsHelper(HWND hwnd) : AbstractWinGraphicsHelper(hwnd)
	{
		for (size_t i = 0; i < 6; i++)
			_brushes[i] = nullptr;
	}

	Win2DGraphicsHelper::~Win2DGraphicsHelper()
	{
		discardCustomDeviceDependentResources();
	}

	bool Win2DGraphicsHelper::initializeCustomDeviceDependentResources()
	{
		int resourcesSucceeded = 0;
		HRESULT hr;

		for (size_t i = 0; i < 6; i++)
		{
			if (_brushes[i] == nullptr)
			{
				hr = _renderTarget->CreateSolidColorBrush(translateColorEnum((ColorEnum)i), &_brushes[i]);

				if (SUCCEEDED(hr))
					resourcesSucceeded++;
			}
		}

		if (resourcesSucceeded == 6)
		{
			return true;
		}
		else
		{
			discardCustomDeviceDependentResources();
			return true;
		}
	}

	void Win2DGraphicsHelper::discardCustomDeviceDependentResources()
	{
		for (size_t i = 0; i < 6; i++)
		{
			if (_brushes[i] != nullptr)
			{
				_brushes[i]->Release();
				_brushes[i] = nullptr;
			}
		}
	}

	bool Win2DGraphicsHelper::checkCustomDeviceDependentResources()
	{
		bool result = true;

		for (size_t i = 0; i < 6; i++)
		{
			if (_brushes[i] == nullptr)
			{
				result = false;
				break;
			}
		}

		return result;
	}

	bool Win2DGraphicsHelper::initializeCustomDeviceIndependentResources()
	{
		return true;
	}

	void Win2DGraphicsHelper::discardCustomDeviceIndependentResources()
	{
		//do nothing
	}

	void Win2DGraphicsHelper::drawSquareColorEnum(int xPos, int yPos, int size, ColorEnum color, bool outline)
	{
		if (_renderTarget && _isWindowPainted)
		{
			D2D1_RECT_F rect;
			rect.top = (float)yPos;
			rect.left = (float)xPos;
			rect.bottom = (float)(yPos + size);
			rect.right = (float)(xPos + size);

			float strokeWidth = 0;
			if (outline)
				strokeWidth = 1.0f;

			_renderTarget->FillRectangle(rect, _brushes[(int)color]);
			if (outline)
				_renderTarget->DrawRectangle(rect, _brushes[0], strokeWidth);
		}
	}
}
