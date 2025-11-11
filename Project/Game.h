#pragma once
#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Sentence.h"
#include "JSONManager.h"
#include <string>
#include <vector>
#include <ctime>

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

    // ユーザー入力文字列：これまでタイプした文字を追跡
    std::string userInput;

    // スコア設定定数
    const int CORRECT_CHAR_POINTS = 10; // 正解文字ごとのポイント
    const int MISTAKE_SCORE_PENALTY = 5; // ミスごとのスコアペナルティ

public:
    // コンストラクタ
    Game();

    // メインメニューを表示する関数
    void showMainMenu();

    // ゲームを開始する関数 - タイピングゲームを初期化して開始
    void start();

    // メインゲームループ関数 - タイピングと画面更新を処理
    void gameLoop();

    // ユーザー入力文字を処理する関数 - 入力と期待される文字を比較
    void processInput(char inputChar);

    // 次の文をランダムに読み込む関数
    void loadNextSentence();

    // コンソール画面をクリアする関数
    void clearScreen() const;
};

#endif
