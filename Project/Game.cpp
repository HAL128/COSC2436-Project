#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <conio.h>

#define NOMINMAX
#include <windows.h>

// コンストラクタ
Game::Game()
{
}

// メインメニューを表示する関数
void Game::showMainMenu()
{
    // 無限ループ - ユーザーが終了を選ぶまで繰り返す
    while (true)
    {
        clearScreen();
        std::cout << "==================================" << std::endl;
        std::cout << "     Welcome to Typing Game!!     " << std::endl;
        std::cout << "==================================" << std::endl;
        std::cout << std::endl;
        std::cout << "1. Play Game" << std::endl;
        std::cout << "2. Exit" << std::endl;
        std::cout << std::endl;
        std::cout << "Enter your choice (1-2): ";

        // 1文字入力を取得 - Enterキー不要
        char choice = _getch();

        switch (choice)
        {
        case '1':
            start();
            break;
        case '2':
            clearScreen();
            std::cout << "Goodbye!" << std::endl;
            return;
            // その他のキーが押された場合
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
            Sleep(1000); // ユーザーがメッセージを読むまで待つ
            break;
        }
    }
}

// ゲームを開始する関数（簡易版）
void Game::start()
{
    clearScreen();
    std::cout << "==================================" << std::endl;
    std::cout << "            Start Game            " << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << std::endl;

    // ユーザー名を入力
    std::string username;
    std::cout << "Enter your username: ";
    // ユーザー名の入力行を取得
    std::getline(std::cin, username);
    player.setUsername(username);
    player.resetScore();

    // JSONファイルから文のデータを読み込む
    std::vector<std::string> sentences = jsonManager.loadSentences("../../Project/sentences.json");

    // 文が読み込めたかチェック
    if (sentences.empty())
    {
        std::cout << "Error: Could not load sentences!" << std::endl;
        std::cout << "Press any key to return to menu..." << std::endl;
        (void)_getch();
        return;
    }

    allSentences = sentences;

    std::cout << std::endl;
    std::cout << "Loaded " << sentences.size() << " sentences successfully!" << std::endl;
    std::cout << std::endl;
    std::cout << "Game will start soon..." << std::endl;
    std::cout << "(Game loop will be implemented in the next step)" << std::endl;
    std::cout << std::endl;
    std::cout << "Press any key to return to menu..." << std::endl;
    (void)_getch();
}

// コンソール画面をクリアする関数
void Game::clearScreen() const
{
    system("cls");
}
