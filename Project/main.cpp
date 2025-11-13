// COSC 2436 Project - Haruto Hirata
#include "Game.h"
#include <iostream>

int main()
{
    try
    {
        // Create a Game object
        Game game;
        // Show main menu
        game.showMainMenu();
    }
    catch (const std::exception& e) // if any exception occurs
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
