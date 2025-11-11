#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>

#define NOMINMAX
#include <windows.h>

// コンストラクタ
Game::Game() : userInput("")
{
    // 文をシャッフルするため乱数生成器を初期化
    srand(static_cast<unsigned int>(time(nullptr)));
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

// ゲームを開始する関数
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

    clearScreen();
    std::cout << "==================================" << std::endl;
    std::cout << "        Game Instructions         " << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "Type the sentences correctly" << std::endl;
    std::cout << std::endl;
    std::cout << "Correct character: +" << CORRECT_CHAR_POINTS << " points" << std::endl;
    std::cout << "Mistake: -" << MISTAKE_SCORE_PENALTY << " points" << std::endl;
    std::cout << std::endl;
    std::cout << "Press ESC to quit early" << std::endl;
    std::cout << std::endl;
    std::cout << "Press any key to start..." << std::endl;
    (void)_getch();

    // タイピングゲームをここから開始
    gameLoop();

    // ゲーム終了後、結果を表示
    clearScreen();
    std::cout << "==================================" << std::endl;
    std::cout << "            Game Over!            " << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << std::endl;
    std::cout << "Player: " << player.getUsername() << std::endl;
    std::cout << "Final Score: " << player.getScore() << " points" << std::endl;
    std::cout << std::endl;
    std::cout << "Press any key to return to menu..." << std::endl;
    (void)_getch();
}

// メインゲームループ関数 - タイピングと画面更新を処理
void Game::gameLoop()
{
    // 最初の文をランダムに読み込む
    loadNextSentence();

    int sentencesCompleted = 0; // 完了した文の数をカウント
    const int MAX_SENTENCES = 3; // この簡易版では3文ま

    // プレイヤー情報を表示
    clearScreen();
    std::cout << "==================================" << std::endl;
    std::cout << "   Player: " << player.getUsername() << " | Score: " << player.getScore() << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << std::endl;
    std::cout << "Type the following sentence:" << std::endl;
    std::cout << std::endl;

    // 現在の文を進捗色付きで表示
    currentSentence.display();
    std::cout << std::endl;

    // 初期のユーザー入力行を表示
    std::cout << "Your input: " << userInput << std::endl;

    // 3文完了するまでループ
    while (sentencesCompleted < MAX_SENTENCES)
    {
        // キーが押されたかチェック
        if (_kbhit())
        {
            // 押されたキーの文字を取得
            char ch = _getch();

            // ESCキーが押されたらbreak
            if (ch == 27)
            {
                break;
            }

            // ユーザー入力文字をチェックして処理
            processInput(ch);

            // 完了した場合、次の文を読み込む
            if (currentSentence.isComplete())
            {
                sentencesCompleted++;
                userInput.clear();

                if (sentencesCompleted < MAX_SENTENCES)
                {
                    loadNextSentence();
                }
            }

            // 画面を更新
            clearScreen();
            std::cout << "==================================" << std::endl;
            std::cout << "   Player: " << player.getUsername() << " | Score: " << player.getScore() << std::endl;
            std::cout << "==================================" << std::endl;
            std::cout << std::endl;
            std::cout << "Sentences completed: " << sentencesCompleted << "/" << MAX_SENTENCES << std::endl;
            std::cout << std::endl;
            std::cout << "Type the following sentence:" << std::endl;
            std::cout << std::endl;

            if (sentencesCompleted < MAX_SENTENCES)
            {
                currentSentence.display();
                std::cout << std::endl;
                std::cout << "Your input: " << userInput << std::endl;
            }
            else
            {
                std::cout << "All sentences completed!" << std::endl;
            }
        }
    }
}

// ユーザー入力文字を処理する関数
void Game::processInput(char inputChar)
{
    // 現在の文から期待される次の文字を取得
    char expectedChar = currentSentence.getNextChar();

    // 入力文字と期待される文字を比較
    if (inputChar == expectedChar)
    {
        player.addScore(CORRECT_CHAR_POINTS);
        userInput += inputChar;
        currentSentence.incrementIndex(); // 次の文字に移動
    }
    else
    {
        // ミスの場合ペナルティを適用
        player.addScore(-MISTAKE_SCORE_PENALTY);
    }
}

// 次の文をランダムに読み込む関数
void Game::loadNextSentence()
{
    // ランダムなインデックスを生成
    int randomIndex = rand() % allSentences.size();
    // 選択された文を取得
    std::string selectedSentence = allSentences[randomIndex];

    currentSentence.setText(selectedSentence);
}

// コンソール画面をクリアする関数
void Game::clearScreen() const
{
    system("cls");
}
