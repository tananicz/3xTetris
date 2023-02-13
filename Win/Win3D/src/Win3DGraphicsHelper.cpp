#include "../headers/Win3DGraphicsHelper.h"

#include <cmath>

using namespace Console2D;

namespace Win3D
{
	Win3DGraphicsHelper::Win3DGraphicsHelper(HWND hwnd) : AbstractWinGraphicsHelper(hwnd)
	{
		_pathGeometry = nullptr;

		for (size_t i = 0; i < 6; i++)
		{
			_fillBrushes[i] = nullptr;
			_shadowBrushes[i] = nullptr;
		}

		initShadowColors();
	}

	Win3DGraphicsHelper::~Win3DGraphicsHelper()
	{
		discardCustomDeviceIndependentResources();
		discardCustomDeviceDependentResources();
	}

	void Win3DGraphicsHelper::initShadowColors()
	{
		_shadowColors[0].r = 0; _shadowColors[0].g = 0; _shadowColors[0].b = 0; _shadowColors[0].a = 1;
		_shadowColors[1].r = 0.76f; _shadowColors[1].g = 0.76f; _shadowColors[1].b = 0.76f; _shadowColors[1].a = 1;
		_shadowColors[2].r = 0.76f; _shadowColors[2].g = 0.76f; _shadowColors[2].b = 0; _shadowColors[2].a = 1;
		_shadowColors[3].r = 0.76f; _shadowColors[3].g = 0; _shadowColors[3].b = 0; _shadowColors[3].a = 1;
		_shadowColors[4].r = 0; _shadowColors[4].g = 0.76f; _shadowColors[4].b = 0; _shadowColors[4].a = 1;
		_shadowColors[5].r = 0; _shadowColors[5].g = 0; _shadowColors[5].b = 0.76f; _shadowColors[5].a = 1;
	}

	int Win3DGraphicsHelper::getSafeDrawingAreaSideSize()
	{
		return _safeDrawingAreaSideSize;
	}

	int Win3DGraphicsHelper::getCubeSideSize()
	{
		return _cubeSideSize;
	}

	void Win3DGraphicsHelper::setupPaintConfig(int wellSideSize, int wellDepth)
	{
		if (_drawingAreaHeight == -1 || _drawingAreaWidth == -1 || _cubeSideSize == -1 || _safeDrawingAreaSideSize == -1 || _perspectiveDist == -1)
		{
			RECT clientRect;
			GetClientRect(_hwnd, &clientRect);

			_drawingAreaWidth = clientRect.right;
			_drawingAreaHeight = (int)(clientRect.bottom * ((float)(100 - _INFO_PANEL_PERCENTAGE))/100);

			int screenSizeTmp = min(_drawingAreaWidth, _drawingAreaHeight) - _PADDING_VAL;

			_cubeSideSize = floor(screenSizeTmp / wellSideSize);
			_safeDrawingAreaSideSize = _cubeSideSize * wellSideSize;
			int bottomSize = (int)(_WELL_BOTTOM_PERCENTAGE * _safeDrawingAreaSideSize);
			_perspectiveDist = (wellDepth * _cubeSideSize * bottomSize) / (_safeDrawingAreaSideSize - bottomSize);
		}
	}

	void Win3DGraphicsHelper::drawCube(int col, int row, int depth, ColorEnum color)
	{
		//TODO
	}

	void Win3DGraphicsHelper::draw3DWalls(int wellDepth)
	{
		int drawAreaSideSize = getSafeDrawingAreaSideSize();
		int cubeSideSize = getCubeSideSize();
		D2D1::ColorF color = D2D1::ColorF(D2D1::ColorF::Red);

		//preparing points
		Point p1 = get2DCoords(-1 * drawAreaSideSize / 2, drawAreaSideSize / 2, 0);
		Point p2 = get2DCoords(drawAreaSideSize / 2, drawAreaSideSize / 2, 0);
		Point p3 = get2DCoords(drawAreaSideSize / 2, -1 * drawAreaSideSize / 2, 0);
		Point p4 = get2DCoords(-1 * drawAreaSideSize / 2, -1 * drawAreaSideSize / 2, 0);

		Point pp1 = get2DCoords(-1 * drawAreaSideSize / 2, drawAreaSideSize / 2, cubeSideSize * wellDepth);
		Point pp2 = get2DCoords(drawAreaSideSize / 2, drawAreaSideSize / 2, cubeSideSize * wellDepth);
		Point pp3 = get2DCoords(drawAreaSideSize / 2, -1 * drawAreaSideSize / 2, cubeSideSize * wellDepth);
		Point pp4 = get2DCoords(-1 * drawAreaSideSize / 2, -1 * drawAreaSideSize / 2, cubeSideSize * wellDepth);

		//proper drawing
		drawLine(p1.x, p1.y, p2.x, p2.y, color);
		drawLine(p2.x, p2.y, p3.x, p3.y, color);
		drawLine(p3.x, p3.y, p4.x, p4.y, color);
		drawLine(p4.x, p4.y, p1.x, p1.y, color);

		drawLine(pp1.x, pp1.y, pp2.x, pp2.y, color);
		drawLine(pp2.x, pp2.y, pp3.x, pp3.y, color);
		drawLine(pp3.x, pp3.y, pp4.x, pp4.y, color);
		drawLine(pp4.x, pp4.y, pp1.x, pp1.y, color);

		drawLine(p1.x, p1.y, pp1.x, pp1.y, color);
		drawLine(p2.x, p2.y, pp2.x, pp2.y, color);
		drawLine(p3.x, p3.y, pp3.x, pp3.y, color);
		drawLine(p4.x, p4.y, pp4.x, pp4.y, color);
	}

	Point Win3DGraphicsHelper::get2DCoords(int x, int y, int z)
	{
		Point result;
		float factor = (float) _perspectiveDist / (_perspectiveDist + z);

		result.x = factor * x;
		result.y = factor * y;

		//transformations to fit within window:
		result.y = -1 * result.y;
		result.moveBy(getDrawingOffset());

		return result;
	}

	Point Win3DGraphicsHelper::getDrawingOffset()
	{
		Point result;

		result.x = _drawingAreaWidth / 2;
		result.y = _drawingAreaHeight / 2;

		return result;
	}

	bool Win3DGraphicsHelper::initializeCustomDeviceDependentResources()
	{
		HRESULT hr;
		int resourcesSucceeded = 0;

		for (size_t i = 0; i < 6; i++)
		{
			hr = _renderTarget->CreateSolidColorBrush(translateColorEnum((ColorEnum) i), &_fillBrushes[i]);
			if (SUCCEEDED(hr))
				resourcesSucceeded++;

			hr = _renderTarget->CreateSolidColorBrush(_shadowColors[i], &_shadowBrushes[i]);
			if (SUCCEEDED(hr))
				resourcesSucceeded++;
		}

		if (resourcesSucceeded == 12)
		{
			return true;
		}
		else
		{
			discardCustomDeviceDependentResources();
			return false;
		}
	}

	bool Win3DGraphicsHelper::checkCustomDeviceDependentResources()
	{
		bool result = true;

		for (size_t i = 0; i < 6; i++)
		{
			if (_fillBrushes[i] == nullptr || _shadowBrushes[i] == nullptr)
			{
				result = false;
				break;
			}
		}

		return result;
	}

	void Win3DGraphicsHelper::discardCustomDeviceDependentResources()
	{
		for (size_t i = 0; i < 6; i++)
		{
			if (_fillBrushes[i] != nullptr)
			{
				_fillBrushes[i]->Release();
				_fillBrushes[i] = nullptr;
			}

			if (_shadowBrushes[i] != nullptr)
			{
				_shadowBrushes[i]->Release();
				_shadowBrushes[i] = nullptr;
			}
		}
	}

	bool Win3DGraphicsHelper::initializeCustomDeviceIndependentResources()
	{
		HRESULT hr = _direct2DFactory->CreatePathGeometry(&_pathGeometry);

		if (SUCCEEDED(hr))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void Win3DGraphicsHelper::discardCustomDeviceIndependentResources()
	{
		if (_pathGeometry != nullptr)
		{
			_pathGeometry->Release();
			_pathGeometry = nullptr;
		}
	}
}
