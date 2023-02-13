#ifndef ABSTRACT_WIN_GRAPHICS_HELPER_H
#define ABSTRACT_WIN_2D_GRAPHICS_HELPER_H

#include "../../Console2D/headers/ColorEnum.h"
#include <d2d1.h>
#include <dwrite.h>

using namespace Console2D;

namespace Win
{
	class AbstractWinGraphicsHelper
	{
	public:
		AbstractWinGraphicsHelper(HWND hwnd);
		~AbstractWinGraphicsHelper();

		bool initializeFactories();
		void beginDraw();
		void endDraw();
		void clear(const D2D1_COLOR_F& color);
		void drawText(const wchar_t* textToDraw, int textLength, int dipSize, int xPos, int yPos, const D2D1_COLOR_F& color);
		void drawRectangleCustom(int xPos, int yPos, int width, int height, const D2D1_COLOR_F& fillColor, bool outline, const D2D1_COLOR_F& outlineColor);
		void drawLine(int xStartPos, int yStartPos, int xEndPos, int yEndPos, const D2D1_COLOR_F& color);

	protected:
		HWND _hwnd;
		ID2D1Factory* _direct2DFactory;
		IDWriteFactory* _writeFactory;
		ID2D1HwndRenderTarget* _renderTarget;
		bool _isWindowPainted;
		D2D1_COLOR_F translateColorEnum(ColorEnum color);
		virtual bool initializeCustomResources() = 0;
		virtual bool checkCustomResources() = 0;
		virtual void discardCustomResources() = 0;

	private:
		PAINTSTRUCT _ps;
		bool initializeResources();
		void discardResources();
	};
}

#endif
