#pragma once
#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Sentence.h"
#include "JSONManager.h"
#include <string>
#include <vector>
#include <chrono>

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

    // ゲーム状態変数
    int remainingTime; // 残り時間（秒）
    int timePenalty; // 累積時間ペナルティ（秒）
    const int TOTAL_TIME = 120; // 総ゲーム時間（秒）
    const int CORRECT_CHAR_POINTS = 10; // 正解文字ごとのポイント
    const int COMPLETE_SENTENCE_BONUS = 20; // 文完了ボーナス
    const int MISTAKE_TIME_PENALTY = 2; // ミスごとの秒ペナルティ
    const int MISTAKE_SCORE_PENALTY = 5; // ミスごとのスコアペナルティ

    // ユーザー入力文字列：これまでタイプした文字を追跡
    std::string userInput;

    // 経過時間計算のための開始時刻
    std::chrono::steady_clock::time_point startTime;

public:
    // コンストラクタ
    Game();

    // メインメニューを表示する関数
    void showMainMenu();

    // ゲームを開始する関数 - タイピングゲームを初期化して開始
    void start();

    // メインゲームループ関数 - タイピングと画面更新を処理
    void gameLoop();

    // 残り時間バーを表示する関数
    void displayTimeBar() const;

    // scores.jsonからトップNのランキングを表示する関数
    void displayRanking(int topN = 10) const;

    // ユーザー入力文字を処理する関数 - 入力と期待される文字を比較
    void processInput(char inputChar);

    // 次の文をランダムに読み込む関数
    void loadNextSentence();

    // コンソール画面をクリアする関数
    void clearScreen() const;

    // 経過時間を秒単位で取得
    int getElapsedTime() const;

    // 残り時間を更新
    void updateRemainingTime();
};

#endif
