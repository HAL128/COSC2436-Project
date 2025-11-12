#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>

#define NOMINMAX
#include <windows.h>

// コンストラクタ
Game::Game() : remainingTime(TOTAL_TIME), timePenalty(0), userInput("")
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
        std::cout << "2. View Top 5 Rankings" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << std::endl;
        std::cout << "Enter your choice (1-3): ";

        // 1文字入力を取得 - Enterキー不要
        char choice = _getch();

        switch (choice)
        {
        case '1':
            start();
            break;
        case '2':
            clearScreen();
            // トップ5のランキングを表示
            displayRanking(5);
            std::cout << std::endl;
            std::cout << "Press any key to return to menu..." << std::endl;
            (void)_getch();
            break;
        case '3':
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
    std::cout << "You have 2 minutes to type as many sentences as possible" << std::endl;
    std::cout << std::endl;
    std::cout << "Correct character: +" << CORRECT_CHAR_POINTS << " points" << std::endl;
    std::cout << "Complete sentence : +" << COMPLETE_SENTENCE_BONUS << " bonus points" << std::endl;
    std::cout << "Mistake: -" << MISTAKE_TIME_PENALTY << " seconds & -" << MISTAKE_SCORE_PENALTY << " points" << std::endl;
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

    // スコアをJSONファイルに保存
    player.updateScoreDB("../../Project/scores.json", jsonManager);

    std::cout << std::endl;
    std::cout << "Press any key to return to menu..." << std::endl;
    (void)_getch();
}

// メインゲームループ関数 - タイピングと画面更新を処理
void Game::gameLoop()
{
    // 最初の文をランダムに読み込む
    loadNextSentence();

    // 開始時刻を記録
    startTime = std::chrono::steady_clock::now();

    // 残り時間とペナルティをリセット
    remainingTime = TOTAL_TIME;
    timePenalty = 0;

    // プレイヤー情報を表示
    clearScreen();
    std::cout << "==================================" << std::endl;
    std::cout << "   Player: " << player.getUsername() << " | Score: " << player.getScore() << std::endl;
    std::cout << "==================================" << std::endl;

    // 残り時間バーを表示
    displayTimeBar();
    std::cout << std::endl;
    std::cout << "Type the following sentence:" << std::endl;
    std::cout << std::endl;

    // 現在の文を進捗色付きで表示
    currentSentence.display();
    std::cout << std::endl;

    // 初期のユーザー入力行を表示
    std::cout << "Your input: " << userInput << std::endl;

    // 時間が切れるまでループ
    while (remainingTime > 0)
    {
        // 経過時間に基づいて残り時間を計算
        updateRemainingTime();

        // 時間切れならループを抜ける
        if (remainingTime <= 0)
        {
            break;
        }

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
                // 文完了のボーナスポイントを追加
                player.addScore(COMPLETE_SENTENCE_BONUS);
                userInput.clear();

                loadNextSentence();
            }

            // 画面を更新
            clearScreen();
            std::cout << "==================================" << std::endl;
            std::cout << "   Player: " << player.getUsername() << " | Score: " << player.getScore() << std::endl;
            std::cout << "==================================" << std::endl;

            displayTimeBar();
            std::cout << std::endl;
            std::cout << "Type the following sentence:" << std::endl;
            std::cout << std::endl;

            currentSentence.display();
            std::cout << std::endl;
            std::cout << "Your input: " << userInput << std::endl;
        }

        // CPU使用率を抑えるため少し待つ
        Sleep(50);
    }
}

// 残り時間をプログレスバーとして表示
void Game::displayTimeBar() const
{
    int barLength = 50;
    // 残り時間に基づいて塗りつぶし長さを計算
    int filledLength = (remainingTime * barLength) / TOTAL_TIME;

    std::cout << "Time: [";
    for (int i = 0; i < barLength; ++i)
    {
        // 塗りつぶし長さ以内かチェック
        if (i < filledLength)
        {
            std::cout << "=";
        }
        else
        {
            std::cout << " ";
        }
    }
    // 残り時間を秒で表示
    std::cout << "] " << remainingTime << "s" << std::endl;
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
        timePenalty += MISTAKE_TIME_PENALTY;
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

// scores.jsonからトップ5のランキングを表示
void Game::displayRanking(int topN) const
{
    // JSONファイルからスコアを読み込む
    std::vector<PlayerScore> scores = jsonManager.loadScores("../../Project/scores.json");

    // ランキングタイトルを表示
    std::cout << "==================================" << std::endl;
    std::cout << "         Ranking (Top " << topN << ")" << std::endl;
    std::cout << "==================================" << std::endl;

    // スコアがない場合のチェック
    if (scores.empty())
    {
        std::cout << "No scores yet!" << std::endl;
        return;
    }

    // トップ5のスコアを表示
    int rank = 1;

    // 各スコアエントリーに対して
    for (const auto& score : scores)
    {
        // ランク、ユーザー名、スコアを表示
        std::cout << rank << ". " << score.username << " - " << score.score << " points" << std::endl;

        rank++;
        if (rank > topN)
            break;
    }
}

// コンソール画面をクリアする関数
void Game::clearScreen() const
{
    system("cls");
}

// ゲーム開始からの経過時間を取得
int Game::getElapsedTime() const
{
    // 現在時刻を取得
    auto currentTime = std::chrono::steady_clock::now();
    // 経過時間を秒単位で計算
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime);
    return static_cast<int>(elapsed.count()); // 秒を返す
}

// 経過時間とペナルティに基づいて残り時間を更新
void Game::updateRemainingTime()
{
    // 開始からの経過時間を取得
    int elapsed = getElapsedTime();
    // 残り時間 = 総時間 - (経過時間 + ペナルティ)
    remainingTime = TOTAL_TIME - elapsed - timePenalty;

    // 残り時間がマイナスなら0にセット
    if (remainingTime < 0)
    {
        remainingTime = 0;
    }
}
