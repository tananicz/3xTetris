#include "../headers/Win2DActionListener.h"

#include "../headers/Win2DAction.h"
#include "../headers/Win2DActionEnum.h"
#include <Windows.h>

namespace Win2D
{
    Win2DActionListener::Win2DActionListener(HWND windowHandle)
    {
        _windowHandle = windowHandle;
    }

    IAction* Win2DActionListener::listenForAction(bool waitForInput)
    {
        //we make no use of waitForInput in this implementation

        MSG msg = { };

        if (PeekMessage(&msg, _windowHandle, 0, 0, PM_REMOVE))
        {
            switch (msg.message)
            {
            case WM_KEYDOWN:
                switch (msg.wParam)
                {
                case VK_ESCAPE:
                    return createAction(ExitGame);
                case 0x53:
                    return createAction(StartGame);
                case VK_LEFT:
                    return createAction(MoveLeft);
                case VK_RIGHT:
                    return createAction(MoveRight);
                case VK_DOWN:
                    return createAction(MoveDown);
                case 0x41:
                    return createAction(RotateLeft);
                case 0x5A:
                    return createAction(RotateRight);
                default:
                    return nullptr;
                }
            case WM_PAINT:
                return createAction(Refresh);
            case WM_CLOSE:
                /*
                A little explanation here - when user closes the window it is not WM_CLOSE sent to message queue, it's WM_NCLBUTTONDOWN instead. In fact PeekMessage never
                gets WM_CLOSE in such a way. So, the solution here in this method is to pass WM_NCLBUTTONDOWN to default window procedure (via DispatchMessage below).
                This will trigger some system operations that will eventually call window procedure with WM_CLOSE message (it omits message queue, but one can still catch it
                inside window procedure). In such case we intercept such message inside window procedure and call PostMessage that will force WM_CLOSE to be put inside
                message queue. We catch it here, in this particular 'case' clause - it will generate our IAction object with exit state set to true. That will allow our
                program to end in a proper way. Otherwise we'd have only window closed but still reside inside our custom message loop.
                */
                return createAction(ExitGame);
            default:
                TranslateMessage(&msg);
                DispatchMessage(&msg);
                return nullptr;
            }
        }
        else
            return nullptr;
    }

    IAction* Win2DActionListener::createAction(Win2DActionEnum actionName)
    {
        Win2DAction* action = new Win2DAction;
        action->actionName = actionName;
        return action;
    }
}
