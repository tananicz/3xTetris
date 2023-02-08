#ifndef WIN_2D_GRAPHICS_HELPER_H
#define WIN_2D_GRAPHICS_HELPER_H

#include "../../Console2D/headers/ColorEnum.h"
#include <d2d1.h>
#include <dwrite.h>

using namespace Console2D;

namespace Win2D
{
	class Win2DGraphicsHelper
	{
	public:
		Win2DGraphicsHelper(HWND hwnd);
		~Win2DGraphicsHelper();

		bool initializeFactories();
		void beginDraw();
		void endDraw();
		void clear(const D2D1_COLOR_F& color);
		void drawText(const wchar_t* textToDraw, int textLength, int dipSize, int xPos, int yPos, const D2D1_COLOR_F& color);
		void drawRectangleCustom(int xPos, int yPos, int width, int height, const D2D1_COLOR_F& color, bool outline);
		void drawSquareColorEnum(int xPos, int yPos, int size, ColorEnum color, bool outline);
		void drawLine(int xStartPos, int yStartPos, int xEndPos, int yEndPos, const D2D1_COLOR_F& color);

	protected:
		HWND _hwnd;
		ID2D1Factory* _direct2DFactory;
		IDWriteFactory* _writeFactory;
		ID2D1HwndRenderTarget* _renderTarget;
		bool _isWindowPainted;
		template <class T> static void safeRelease(T** ppT);

	private:
		ID2D1SolidColorBrush* _brushes[6];
		PAINTSTRUCT _ps;
		bool checkBrushes();
		D2D1_COLOR_F translateColorEnum(ColorEnum color);
		bool initializeResources();
		void discardResources();
	};
}

#endif
