// COSC 2436 Project - Haruto Hirata
#include "Player.h"
#include <iostream>

int main()
{
    // Playerクラスのテスト
    std::cout << "=== Player Class Test ===" << std::endl;
    std::cout << std::endl;

    // プレイヤーを作成
    Player player;
    player.setUsername("TestUser");

    std::cout << "Player: " << player.getUsername() << std::endl;
    std::cout << "Initial Score: " << player.getScore() << std::endl;
    std::cout << std::endl;

    // スコアを追加
    player.addScore(100);
    std::cout << "After adding 100 points: " << player.getScore() << std::endl;

    player.addScore(50);
    std::cout << "After adding 50 points: " << player.getScore() << std::endl;

    // マイナスポイント（ペナルティ）を追加
    player.addScore(-30);
    std::cout << "After penalty -30 points: " << player.getScore() << std::endl;
    std::cout << std::endl;

    // スコアをリセット
    player.resetScore();
    std::cout << "After reset: " << player.getScore() << std::endl;

    return 0;
}
