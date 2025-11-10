// COSC 2436 Project - Haruto Hirata
#include "Game.h"
#include <iostream>

int main()
{
    try
    {
        // Gameオブジェクトを作成
        Game game;
        // メインメニューを表示
        game.showMainMenu();
    }
    catch (const std::exception& e) // 例外が発生した場合
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
