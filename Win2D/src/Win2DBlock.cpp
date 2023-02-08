#include "../headers/Win2DBlock.h"

#include "../headers/Win2DAction.h"
#include "../headers/Win2DActionEnum.h"
#include "../../Console2D/headers/UserAction2D.h"
#include "../../Console2D/headers/ActionEnum.h"

namespace Win2D
{
	bool Win2DBlock::tryAction(IBoard* board, IAction* action)
	{
		Win2DAction* win2DAction = static_cast<Win2DAction*>(action);
		Win2DActionEnum actionName = win2DAction->actionName;

		Console2D::UserAction2D console2DAction;

		switch (actionName)
		{
		case MoveLeft:
			console2DAction.actionName = Console2D::MoveLeft;
			break;
		case MoveRight:
			console2DAction.actionName = Console2D::MoveRight;
			break;
		case MoveDown:
			console2DAction.actionName = Console2D::MoveDown;
			break;
		case RotateLeft:
			console2DAction.actionName = Console2D::RotateLeft;
			break;
		case RotateRight:
			console2DAction.actionName = Console2D::RotateRight;
			break;
		default:
			console2DAction.actionName = Console2D::NoAction;
			break;
		}

		return TextBlock::tryAction(board, &console2DAction);
	}
}
