#include "../headers/AbstractWinGraphicsHelper.h"

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

namespace Win
{
	AbstractWinGraphicsHelper::AbstractWinGraphicsHelper(HWND hwnd)
	{
		_hwnd = hwnd;
		_direct2DFactory = nullptr;
		_writeFactory = nullptr;
		_renderTarget = nullptr;
		_isWindowPainted = false;
		_ps = { };
	}

	AbstractWinGraphicsHelper::~AbstractWinGraphicsHelper()
	{
		if (_direct2DFactory != nullptr)
		{
			_direct2DFactory->Release();
			_direct2DFactory = nullptr;
		}

		if (_writeFactory != nullptr)
		{
			_writeFactory->Release();
			_writeFactory = nullptr;
		}

		if (_renderTarget != nullptr)
		{
			_renderTarget->Release();
			_renderTarget = nullptr;
		}
	}

	bool AbstractWinGraphicsHelper::initializeFactories()
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
					_direct2DFactory->Release();
					_direct2DFactory = nullptr;

					return false;
				}
			}
			else
				return false;
		}
		else
			return false;
	}

	bool AbstractWinGraphicsHelper::initializeResources()
	{
		//we don't check for _direct2DFactory != null here
		//we assume that if we're at this point of the program, initializeFactories must have returned true at it's beginning in main.cpp

		bool customResourcesCheck = checkCustomResources();

		if (_renderTarget && customResourcesCheck)
		{
			return true;
		}
		else
		{
			bool continueInit = false;
			HRESULT hr;

			if (!_renderTarget)
			{
				RECT rc;
				GetClientRect(_hwnd, &rc);
				D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

				hr = _direct2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(_hwnd, size), &_renderTarget);
				if (SUCCEEDED(hr))
					continueInit = true;
			}
			else
				continueInit = true;

			if (continueInit)
			{
				if (customResourcesCheck)
				{
					return true;
				}
				else
				{
					if (initializeCustomResources())
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
			else
			{
				discardResources();
				return false;
			}
		}
	}

	void AbstractWinGraphicsHelper::discardResources()
	{
		if (_renderTarget != nullptr)
		{
			_renderTarget->Release();
			_renderTarget = nullptr;
		}

		discardCustomResources();
	}

	void AbstractWinGraphicsHelper::beginDraw()
	{
		if (!_isWindowPainted && initializeResources())
		{
			_isWindowPainted = true;
			BeginPaint(_hwnd, &_ps);
			_renderTarget->BeginDraw();
		}
	}

	void AbstractWinGraphicsHelper::endDraw()
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

	void AbstractWinGraphicsHelper::clear(const D2D1_COLOR_F& color)
	{
		if (_renderTarget && _isWindowPainted)
			_renderTarget->Clear(color);
	}

	void AbstractWinGraphicsHelper::drawText(const wchar_t* textToDraw, int textLength, int dipSize, int xPos, int yPos, const D2D1_COLOR_F& color)
	{
		if (_renderTarget && _writeFactory && _isWindowPainted)
		{
			IDWriteTextFormat* textFormat = nullptr;
			HRESULT hr = _writeFactory->CreateTextFormat(L"Verdana", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, (FLOAT)dipSize, L"", &textFormat);

			if (SUCCEEDED(hr))
			{
				ID2D1SolidColorBrush* brush = nullptr;
				hr = _renderTarget->CreateSolidColorBrush(color, &brush);

				if (SUCCEEDED(hr))
				{
					float dpi = (float)GetDpiForWindow(_hwnd);
					float fontHeight = dipSize * (dpi / 96.0f);
					D2D1_SIZE_F renderTargetSize = _renderTarget->GetSize();

					D2D1_RECT_F rect;
					rect.top = (float)yPos;
					rect.left = (float)xPos;
					rect.bottom = yPos + fontHeight * 1.3f;
					rect.right = renderTargetSize.width;
					_renderTarget->DrawText(textToDraw, textLength, textFormat, rect, brush);

					brush->Release();
				}

				textFormat->Release();
			}
		}
	}

	void AbstractWinGraphicsHelper::drawRectangleCustom(int xPos, int yPos, int width, int height, const D2D1_COLOR_F& fillColor, bool outline, const D2D1_COLOR_F& outlineColor)
	{
		if (_renderTarget && _isWindowPainted)
		{
			D2D1_RECT_F rect;
			rect.top = (float)yPos;
			rect.left = (float)xPos;
			rect.bottom = (float)(yPos + height);
			rect.right = (float)(xPos + width);

			ID2D1SolidColorBrush* fillBrush = nullptr;
			HRESULT hr = _renderTarget->CreateSolidColorBrush(fillColor, &fillBrush);

			if (SUCCEEDED(hr))
			{
				_renderTarget->FillRectangle(rect, fillBrush);

				if (outline)
				{ 
					ID2D1SolidColorBrush* outlineBrush = nullptr;
					hr = _renderTarget->CreateSolidColorBrush(outlineColor, &outlineBrush);

					if (SUCCEEDED(hr))
					{
						_renderTarget->DrawRectangle(rect, outlineBrush, 1.0f);
						outlineBrush->Release();
					}
				}

				fillBrush->Release();
			}
		}
	}

	void AbstractWinGraphicsHelper::drawLine(int xStartPos, int yStartPos, int xEndPos, int yEndPos, const D2D1_COLOR_F& color)
	{
		if (_renderTarget && _isWindowPainted)
		{
			ID2D1SolidColorBrush* brush = nullptr;
			HRESULT hr = _renderTarget->CreateSolidColorBrush(color, &brush);

			if (SUCCEEDED(hr))
			{
				_renderTarget->DrawLine(D2D1::Point2F((float)xStartPos, (float)yStartPos), D2D1::Point2F((float)xEndPos, (float)yEndPos), brush);
			}
		}
	}
}
