#pragma once
#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Sentence.h"
#include "JSONManager.h"
#include <string>
#include <vector>

// ゲーム全体を管理するクラス
class Game
{
private:
    // プレイヤーオブジェクト：名前とスコアを保持
    Player player;

    // 現在タイプする文
    Sentence currentSentence;

    // ファイル操作用のJSONマネージャー
    JSONManager jsonManager;

    // JSONファイルから読み込んだ全ての文
    std::vector<std::string> allSentences;

public:
    // コンストラクタ
    Game();

    // メインメニューを表示する関数
    void showMainMenu();

    // ゲームを開始する関数 - タイピングゲームを初期化して開始
    void start();

    // コンソール画面をクリアする関数
    void clearScreen() const;
};

#endif
