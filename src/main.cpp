#include "Game.hpp"


int main()
{
    // Game init
    std::unique_ptr<Game> game(new Game());
    
    while (game->isRunning())
    {        
        // Update 
        game->update();
        // Render
        game->render();   
    }

    return EXIT_SUCCESS;
}