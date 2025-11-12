#include "Player.h"
#include <algorithm>
#include <iostream>

// デフォルトコンストラクタ：空の名前とスコア0で初期化
Player::Player() : username(""), score(0) {}

// 名前を指定するコンストラクタ
Player::Player(const std::string& name) : username(name), score(0) {}

// スコアにポイントを追加する関数
// マイナスのポイントも追加可能（ペナルティとして）
void Player::addScore(int points)
{
    score += points;
    // スコアがマイナスにならないようにする
    if (score < 0)
    {
        score = 0;
    }
}

// ユーザー名を設定する関数
void Player::setUsername(const std::string& name)
{
    username = name;
}

// ユーザー名を取得する関数
std::string Player::getUsername() const
{
    return username;
}

// 現在のスコアを取得する関数
int Player::getScore() const
{
    return score;
}

// スコアを0にリセットする関数
void Player::resetScore()
{
    score = 0;
}

// スコアデータベースとランキングを更新する関数
// @param filename: JSONスコアファイルへのパス
// @param jsonManager: ファイル操作用のJSONManagerインスタンス
void Player::updateScoreDB(const std::string& filename, JSONManager& jsonManager)
{
    // 既存のスコアデータを読み込む
    std::vector<PlayerScore> scores = jsonManager.loadScores(filename);

    // 同じユーザー名が既に存在するかチェック
    bool found = false;
    for (auto& playerScore : scores)
    {
        if (playerScore.username == username)
        {
            // 現在のスコアが既存のスコアより高い場合のみ更新
            if (score > playerScore.score)
            {
                playerScore.score = score;
                std::cout << "New high score for " << username << "!" << std::endl;
            }
            found = true;
            break;
        }
    }

    // 新規ユーザーの場合、リストに追加
    if (!found)
    {
        PlayerScore newScore;
        newScore.username = username;
        newScore.score = score;
        scores.push_back(newScore);
    }

    // スコアを降順にソート（ランキング作成）
    std::sort(scores.begin(), scores.end(), [](const PlayerScore& a, const PlayerScore& b)
        { return a.score > b.score; });

    // 更新されたスコアデータをファイルに保存
    jsonManager.saveScores(filename, scores);
}
