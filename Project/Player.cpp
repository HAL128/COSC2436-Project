#include "Player.h"

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
