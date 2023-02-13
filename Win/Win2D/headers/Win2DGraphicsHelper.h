#ifndef WIN_2D_GRAPHICS_HELPER_H
#define WIN_2D_GRAPHICS_HELPER_H

#include "../../headers/AbstractWinGraphicsHelper.h"
#include "../../../Console2D/headers/ColorEnum.h"
#include <d2d1.h>
#include <dwrite.h>

using namespace Console2D;

namespace Win2D
{
	class Win2DGraphicsHelper : public Win::AbstractWinGraphicsHelper
	{
	public:
		Win2DGraphicsHelper(HWND hwnd);
		~Win2DGraphicsHelper();

		void drawSquareColorEnum(int xPos, int yPos, int size, ColorEnum color, bool outline);

	protected:
		//AbstractWinGraphicsHelper implementations
		bool initializeCustomDeviceDependentResources();
		bool checkCustomDeviceDependentResources();
		void discardCustomDeviceDependentResources();
		bool initializeCustomDeviceIndependentResources();
		void discardCustomDeviceIndependentResources();

	private:
		ID2D1SolidColorBrush* _brushes[6];
	};
}

#endif
