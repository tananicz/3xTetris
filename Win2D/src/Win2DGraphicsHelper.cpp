#include "../headers/Win2DGraphicsHelper.h"

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

namespace Win2D
{
	Win2DGraphicsHelper::Win2DGraphicsHelper(HWND hwnd)
	{
		_hwnd = hwnd;
		_direct2DFactory = nullptr;
		_writeFactory = nullptr;
		_renderTarget = nullptr;
		_isWindowPainted = false;
		_ps = { };

		for (size_t i = 0; i < 6; i++)
			_brushes[i] = nullptr;
	}

	Win2DGraphicsHelper::~Win2DGraphicsHelper()
	{
		safeRelease(&_direct2DFactory);
		safeRelease(&_writeFactory);
		discardResources();
	}

	template <class T> static void Win2DGraphicsHelper::safeRelease(T** ppT)
	{
		if (*ppT)
		{
			(*ppT)->Release();
			*ppT = nullptr;
		}
	}

	bool Win2DGraphicsHelper::initializeFactories()
	{
		if (_direct2DFactory == nullptr && _writeFactory == nullptr && !_isWindowPainted)
		{
			HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &_direct2DFactory);
			if (SUCCEEDED(hr))
			{
				hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(_writeFactory), reinterpret_cast<IUnknown**>(&_writeFactory));
				if (SUCCEEDED(hr))
				{
					return true;
				}
				else
				{
					safeRelease(&_direct2DFactory);
					return false;
				}
			}
			else
				return false;
		}
		else
			return false;
	}

	bool Win2DGraphicsHelper::initializeResources()
	{
		//we don't check for _direct2DFactory != null here
		//we assume that if we're at this point of the program, initializeFactories must have returned true at it's beginning in main.cpp

		if (_renderTarget && checkBrushes())
		{
			return true;
		}
		else
		{
			int resourcesSucceeded = 0;
			HRESULT hr;

			if (!_renderTarget)
			{
				RECT rc;
				GetClientRect(_hwnd, &rc);
				D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

				hr = _direct2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(_hwnd, size), &_renderTarget);

				if (SUCCEEDED(hr))
					resourcesSucceeded++;
			}
			else
				resourcesSucceeded++;

			if (_renderTarget)
			{
				for (size_t i = 0; i < 6; i++)
				{
					if (_brushes[i] == nullptr)
					{
						hr = _renderTarget->CreateSolidColorBrush(translateColorEnum((ColorEnum) i), &_brushes[i]);

						if (SUCCEEDED(hr))
							resourcesSucceeded++;
					}
				}
			}

			if (resourcesSucceeded == 7)
			{
				return true;
			}
			else
			{
				discardResources();
				return false;
			}
		}
	}

	void Win2DGraphicsHelper::discardResources()
	{
		safeRelease(&_renderTarget);

		for (size_t i = 0; i < 6; i++)
			safeRelease(&_brushes[i]);
	}

	D2D1_COLOR_F Win2DGraphicsHelper::translateColorEnum(ColorEnum color)
	{
		switch (color)
		{
		case White:
			return D2D1::ColorF(1.0f, 1.0f, 1.0f);
		case Red:
			return D2D1::ColorF(1.0f, 0, 0);
		case Yellow:
			return D2D1::ColorF(1.0f, 1.0f, 0);
		case Green:
			return D2D1::ColorF(0, 1.0f, 0);
		case Blue:
			return D2D1::ColorF(0, 0, 1.0f);
		default:
			return D2D1::ColorF(0, 0, 0);
		}
	}

	bool Win2DGraphicsHelper::checkBrushes()
	{
		bool result = true;

		for (size_t i = 0; i < 6; i++)
		{
			if (_brushes[i] == nullptr)
				result = false;
		}

		return result;
	}

	void Win2DGraphicsHelper::beginDraw()
	{
		if (!_isWindowPainted && initializeResources())
		{
			_isWindowPainted = true;
			BeginPaint(_hwnd, &_ps);
			_renderTarget->BeginDraw();
		}
	}

	void Win2DGraphicsHelper::endDraw()
	{
		if (_isWindowPainted && _renderTarget)
		{
			HRESULT hr = _renderTarget->EndDraw();

			//handling device loss in the following line
			if (hr == D2DERR_RECREATE_TARGET)
			{
				//we discard resources, next time the window will be painted they'll be created inside beginDraw()
				discardResources();
			}

			EndPaint(_hwnd, &_ps);
			_ps = { };
			_isWindowPainted = false;
		}
	}

	void Win2DGraphicsHelper::clear(const D2D1_COLOR_F& color)
	{
		if (_renderTarget && _isWindowPainted)
			_renderTarget->Clear(color);
	}

	void Win2DGraphicsHelper::drawText(const wchar_t* textToDraw, int textLength, int dipSize, int xPos, int yPos, const D2D1_COLOR_F& color)
	{
		if (_renderTarget && _writeFactory && _isWindowPainted)
		{
			IDWriteTextFormat* textFormat = nullptr;
			HRESULT hr = _writeFactory->CreateTextFormat(L"Verdana", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, (FLOAT) dipSize, L"", &textFormat);

			if (SUCCEEDED(hr))
			{
				ID2D1SolidColorBrush* brush = nullptr;
				hr = _renderTarget->CreateSolidColorBrush(color, &brush);

				if (SUCCEEDED(hr))
				{
					float dpi = (float) GetDpiForWindow(_hwnd);
					float fontHeight = dipSize * (dpi / 96.0f);
					D2D1_SIZE_F renderTargetSize = _renderTarget->GetSize();

					D2D1_RECT_F rect;
					rect.top = (float) yPos;
					rect.left = (float) xPos;
					rect.bottom = yPos + fontHeight * 1.3f;
					rect.right = renderTargetSize.width;
					_renderTarget->DrawText(textToDraw, textLength, textFormat, rect, brush);

					brush->Release();
				}

				textFormat->Release();
			}
		}
	}

	void Win2DGraphicsHelper::drawRectangleCustom(int xPos, int yPos, int width, int height, const D2D1_COLOR_F& color, bool outline)
	{
		if (_renderTarget && _isWindowPainted)
		{
			D2D1_RECT_F rect;
			rect.top = (float) yPos;
			rect.left = (float) xPos;
			rect.bottom = (float) (yPos + height);
			rect.right = (float) (xPos + width);

			ID2D1SolidColorBrush* brush = nullptr;
			HRESULT hr = _renderTarget->CreateSolidColorBrush(color, &brush);

			if (SUCCEEDED(hr))
			{
				_renderTarget->FillRectangle(rect, brush);
				if (outline)
					_renderTarget->DrawRectangle(rect, _brushes[0], 1.0f);

				brush->Release();
			}
		}
	}

	void Win2DGraphicsHelper::drawSquareColorEnum(int xPos, int yPos, int size, ColorEnum color, bool outline)
	{
		if (_renderTarget && _isWindowPainted)
		{
			D2D1_RECT_F rect;
			rect.top = (float) yPos;
			rect.left = (float) xPos;
			rect.bottom = (float) (yPos + size);
			rect.right = (float) (xPos + size);

			float strokeWidth = 0;
			if (outline)
				strokeWidth = 1.0f;

			_renderTarget->FillRectangle(rect, _brushes[(int)color]);
			if (outline)
				_renderTarget->DrawRectangle(rect, _brushes[0], strokeWidth);
		}
	}

	void Win2DGraphicsHelper::drawLine(int xStartPos, int yStartPos, int xEndPos, int yEndPos, const D2D1_COLOR_F& color)
	{
		if (_renderTarget && _isWindowPainted)
		{
			_renderTarget->DrawLine(D2D1::Point2F((float) xStartPos, (float) yStartPos), D2D1::Point2F((float) xEndPos, (float) yEndPos), _brushes[0]);
		}
	}
}
