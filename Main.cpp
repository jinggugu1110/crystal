#include "Game.h"

int main()
{
    
    Game game(1600, 900);
    game.Init();

    while (!WindowShouldClose())
    {
        game.Update();
        game.Draw();
    }
    game.Uninit();
    return 0;
}
