#include "../headers/Win3DGraphicsHelper.h"

#include "../headers/Point3D.h"
#include "../headers/CubeSideEnum.h"
#include <cmath>

using namespace Console2D;

namespace Win3D
{
	Win3DGraphicsHelper::Win3DGraphicsHelper(HWND hwnd) : AbstractWinGraphicsHelper(hwnd)
	{
		_drawingOffset = {};

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

	void Win3DGraphicsHelper::setupPaintConfig(int wellSideSize, int wellDepth)
	{
		if (_drawingAreaHeight == -1 || _drawingAreaWidth == -1 || _cubeSideSize == -1 || _safeDrawingAreaSideSize == -1 || _perspectiveDist == -1)
		{
			RECT clientRect;
			GetClientRect(_hwnd, &clientRect);

			_drawingAreaWidth = clientRect.right;
			_drawingAreaHeight = (int)(clientRect.bottom * ((float)(100 - _INFO_PANEL_PERCENTAGE))/100);

			int screenSizeTmp = min(_drawingAreaWidth, _drawingAreaHeight) - _PADDING_VAL;

			_cubeSideSize = (int) floor(screenSizeTmp / wellSideSize);
			_safeDrawingAreaSideSize = _cubeSideSize * wellSideSize;
			int bottomSize = (int)(((float) _WELL_BOTTOM_PERCENTAGE / 100) * _safeDrawingAreaSideSize);
			_perspectiveDist = (wellDepth * _cubeSideSize * bottomSize) / (_safeDrawingAreaSideSize - bottomSize);

			_drawingOffset.x = _drawingAreaWidth / 2;
			_drawingOffset.y = _drawingAreaHeight / 2;
		}
	}

	void Win3DGraphicsHelper::drawCube(int col, int row, int depth, ColorEnum color)
	{
		ID2D1PathGeometry* pathGeometryFront = nullptr;
		ID2D1PathGeometry* pathGeometrySide = nullptr;
		
		ID2D1GeometrySink* sinkFront = nullptr;
		ID2D1GeometrySink* sinkSide = nullptr;

		HRESULT hr = _direct2DFactory->CreatePathGeometry(&pathGeometryFront);

		if (SUCCEEDED(hr))
		{
			hr = pathGeometryFront->Open(&sinkFront);

			if (SUCCEEDED(hr))
			{
				Point3D coords[4];
				coords[0] = Point3D((-1 * _safeDrawingAreaSideSize / 2) + (col * _cubeSideSize), (_safeDrawingAreaSideSize / 2) - (row * _cubeSideSize), depth * _cubeSideSize);
				coords[1] = coords[0] + Point3D(_cubeSideSize, 0, 0);
				coords[2] = coords[1] + Point3D(0, -1 * _cubeSideSize, 0);
				coords[3] = coords[2] + Point3D(-1 * _cubeSideSize, 0, 0);

				int cubeSides = determineCubeSides(col, row, _safeDrawingAreaSideSize / _cubeSideSize);
				Point p1;

				for (struct { int i; CubeSideEnum side; } k = { 0, Top }; k.i < 4; k.i++, k.side = (CubeSideEnum)(k.side << 1))
				{
					if (k.i == 0)
					{
						p1 = get2DCoords(coords[0]);
						sinkFront->BeginFigure(D2D1::Point2F((float)p1.x, (float)p1.y), D2D1_FIGURE_BEGIN_FILLED);
					}

					int j = (k.i < 3) ? k.i + 1 : 0;
					p1 = get2DCoords(coords[j]);
					sinkFront->AddLine(D2D1::Point2F((float)p1.x, (float)p1.y));

					if (k.side & cubeSides)
					{
						hr = _direct2DFactory->CreatePathGeometry(&pathGeometrySide);

						if (SUCCEEDED(hr))
						{
							hr = pathGeometrySide->Open(&sinkSide);

							if (SUCCEEDED(hr))
							{
								Point p0 = get2DCoords(coords[k.i]);
								Point p2 = get2DCoords(coords[j] + Point3D(0, 0, _cubeSideSize));
								Point p3 = get2DCoords(coords[k.i] + Point3D(0, 0, _cubeSideSize));

								sinkSide->BeginFigure(D2D1::Point2F((float)p0.x, (float)p0.y), D2D1_FIGURE_BEGIN_FILLED);
								sinkSide->AddLine(D2D1::Point2F((float)p1.x, (float)p1.y));
								sinkSide->AddLine(D2D1::Point2F((float)p2.x, (float)p2.y));
								sinkSide->AddLine(D2D1::Point2F((float)p3.x, (float)p3.y));
								sinkSide->AddLine(D2D1::Point2F((float)p0.x, (float)p0.y));
								sinkSide->EndFigure(D2D1_FIGURE_END_CLOSED);
								sinkSide->Close();

								_renderTarget->DrawGeometry(pathGeometrySide, _shadowBrushes[0], 1);
								_renderTarget->FillGeometry(pathGeometrySide, _shadowBrushes[(int)color]);

								sinkSide->Release();
								sinkSide = nullptr;
							}

							pathGeometrySide->Release();
							pathGeometrySide = nullptr;
						}
					}
				}
				
				sinkFront->EndFigure(D2D1_FIGURE_END_CLOSED);
				sinkFront->Close();
				
				_renderTarget->DrawGeometry(pathGeometryFront, _fillBrushes[0], 1);
				_renderTarget->FillGeometry(pathGeometryFront, _fillBrushes[(int)color]);

				sinkFront->Release();
			}

			pathGeometryFront->Release();
		}
	}

	void Win3DGraphicsHelper::draw3DWalls(int wellDepth)
	{
		D2D1::ColorF color = D2D1::ColorF(D2D1::ColorF::Red);

		//preparing points
		Point p1 = get2DCoords(-1 * _safeDrawingAreaSideSize / 2, _safeDrawingAreaSideSize / 2, 0);
		Point p2 = get2DCoords(_safeDrawingAreaSideSize / 2, _safeDrawingAreaSideSize / 2, 0);
		Point p3 = get2DCoords(_safeDrawingAreaSideSize / 2, -1 * _safeDrawingAreaSideSize / 2, 0);
		Point p4 = get2DCoords(-1 * _safeDrawingAreaSideSize / 2, -1 * _safeDrawingAreaSideSize / 2, 0);

		Point pp1 = get2DCoords(-1 * _safeDrawingAreaSideSize / 2, _safeDrawingAreaSideSize / 2, _cubeSideSize * wellDepth);
		Point pp2 = get2DCoords(_safeDrawingAreaSideSize / 2, _safeDrawingAreaSideSize / 2, _cubeSideSize * wellDepth);
		Point pp3 = get2DCoords(_safeDrawingAreaSideSize / 2, -1 * _safeDrawingAreaSideSize / 2, _cubeSideSize * wellDepth);
		Point pp4 = get2DCoords(-1 * _safeDrawingAreaSideSize / 2, -1 * _safeDrawingAreaSideSize / 2, _cubeSideSize * wellDepth);

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

		result.x = (int)(factor * x);
		result.y = (int)(factor * y);

		//transformations to fit within window:
		result.y = -1 * result.y;
		result.moveBy(_drawingOffset);

		return result;
	}

	Point Win3DGraphicsHelper::get2DCoords(Point3D point3D)
	{
		return get2DCoords(point3D.x, point3D.y, point3D.z);
	}

	int Win3DGraphicsHelper::determineCubeSides(int row, int col, int cubesPerSide)
	{
		int result = 0;

		//it will be easier to operate on row and col from range [1 ... cubesPerSide]
		row++; 
		col++;

		if (cubesPerSide % 2 == 0)
		{
			int middle = cubesPerSide / 2;

			if (row < middle)
				result |= CubeSideEnum::Right;
			else if (row > middle + 1)
				result |= CubeSideEnum::Left;

			if (col < middle)
				result |= CubeSideEnum::Bottom;
			else if (col > middle + 1)
				result |= CubeSideEnum::Top;
		}
		else
		{
			int middle = cubesPerSide / 2 + 1;

			if (row < middle)
				result |= CubeSideEnum::Right;
			else if (row > middle)
				result |= CubeSideEnum::Left;

			if (col < middle)
				result |= CubeSideEnum::Bottom;
			else if (col > middle)
				result |= CubeSideEnum::Top;
		}

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
		//all or nothing approach here - we either create all resources and return true or release all d2d interfaces in case of any failure and return false

		return true;
	}

	void Win3DGraphicsHelper::discardCustomDeviceIndependentResources()
	{
		//do nothing
	}
}
