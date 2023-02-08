#ifndef GAME_H
#define GAME_H

#include "../../Interfaces/IGameFactory.h"

class Game
{
public:
    Game(IGameFactory* gameFactory);
    void start();

private:
    IGameFactory* _gameFactory;
    void startProperGame();
};

#endif // GAME_H