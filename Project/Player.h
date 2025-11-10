#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <string>

// プレイヤー情報を管理するクラス
class Player
{
private:
    std::string username; // ユーザー名
    int score; // 現在のスコア

public:
    // デフォルトコンストラクタ：空の名前とスコア0で初期化
    Player();

    // 名前を指定するコンストラクタ
    Player(const std::string& name);

    // スコアにポイントを追加する関数
    void addScore(int points);

    // ユーザー名を設定する関数
    void setUsername(const std::string& name);

    // ユーザー名を取得する関数
    std::string getUsername() const;

    // 現在のスコアを取得する関数
    int getScore() const;

    // スコアを0にリセットする関数
    void resetScore();
};

#endif
