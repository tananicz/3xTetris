#ifndef WIN_3D_GRAPHICS_HELPER_H
#define WIN_3D_GRAPHICS_HELPER_H

#include "../../headers/AbstractWinGraphicsHelper.h"
#include "../headers/Point3D.h"
#include "../../../Console2D/headers/Point.h"
#include "../../../Console2D/headers/ColorEnum.h"

namespace Win3D
{
	class Win3DGraphicsHelper : public Win::AbstractWinGraphicsHelper
	{
	public:
		Win3DGraphicsHelper(HWND hwnd);
		~Win3DGraphicsHelper();

		void setupPaintConfig(int wellSideSize, int wellDepth);
		void drawCube(int col, int row, int depth, ColorEnum color);
		void draw3DWalls(int wellDepth);

	protected:
		//AbstractWinGraphicsHelper implementations
		bool initializeCustomDeviceDependentResources();
		bool checkCustomDeviceDependentResources();
		void discardCustomDeviceDependentResources();
		bool initializeCustomDeviceIndependentResources();

	private:
		static const int _PADDING_VAL = 30;
		static const int _INFO_PANEL_PERCENTAGE = 15;
		static const int _WELL_BOTTOM_PERCENTAGE = 40;
		int _drawingAreaWidth = -1;
		int _drawingAreaHeight = -1;
		int _cubeSideSize = -1;
		int _safeDrawingAreaSideSize = -1;
		int _perspectiveDist = -1;
		Point _drawingOffset;
		ID2D1SolidColorBrush* _fillBrushes[6];
		ID2D1SolidColorBrush* _shadowBrushes[6];
		D2D_COLOR_F _shadowColors[6];
		void initShadowColors();
		int determineCubeSides(int row, int col, int cubesPerSide);
		Point get2DCoords(int x, int y, int z);
		Point get2DCoords(Point3D point3D);
		void discardCustomDeviceIndependentResources();
	};
}

#endif
