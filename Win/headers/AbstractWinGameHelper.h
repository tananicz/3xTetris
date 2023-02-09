#ifndef ABSTRACT_WIN_GAME_HELPER_H
#define ABSTRACT_WIN_GAME_HELPER_H

#include "../../Interfaces/IGameHelper.h"

namespace Win
{
	class AbstractWinGameHelper : public IGameHelper
	{
	public:
		AbstractWinGameHelper();
		~AbstractWinGameHelper();
		
		//IGameHelper implementations
		bool initSystemSettings(HINSTANCE hInstance, int nCmdShow);
		IActionListener* getActionListener();
		IAction* getWelcomeScreenAction();
		IAction* getGameOverScreenAction();
		void drawBoard(IBoard* board, IBlock* block, int points) = 0;
		double updateInterval(int points);

	protected:
		HINSTANCE _hInstance;
		HWND _windowHandle;
		IActionListener* _actionListener;
		wchar_t* intToWcharTPtr(int i);
		virtual const wchar_t* getWindowClassName() = 0;
		virtual const wchar_t* getWindowTitle() = 0;
		virtual void paintWelcomeScreen() = 0;
		virtual void paintGameOverScreen() = 0;

	private:
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};
}

#endif
