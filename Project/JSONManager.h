#pragma once
#ifndef JSONMANAGER_H
#define JSONMANAGER_H

#include <string>
#include <vector>

// プレイヤースコア情報を格納する構造体
struct PlayerScore
{
    std::string username; // ユーザー名
    int score; // スコア
};

// JSON形式のファイルを読み書きするクラス
class JSONManager
{
public:
    // コンストラクタ
    JSONManager();

    // JSONファイルから文のリストを読み込む関数
    // @param filename: JSONファイルのパス
    // @return 文のリスト
    std::vector<std::string> loadSentences(const std::string& filename) const;

    // JSONファイルからスコアのリストを読み込む関数
    // @param filename: JSONファイルのパス
    // @return PlayerScore構造体のリスト
    std::vector<PlayerScore> loadScores(const std::string& filename) const;

    // JSONファイルにスコアのリストを保存する関数
    // @param filename: JSONファイルのパス
    // @param scores: 保存するPlayerScore構造体のリスト
    void saveScores(const std::string& filename, const std::vector<PlayerScore>& scores) const;

private:
    // 文字列の前後の空白を削除する関数
    std::string trim(const std::string& str) const;

    // JSON文字列値から実際の文字列を抽出する関数
    // @param str: JSON文字列値
    // @return ダブルクオートを除いた文字列
    std::string parseString(const std::string& str) const;

    // JSON値の文字列を整数に変換する関数
    // @param str: JSON整数値
    // @return パースされた整数
    int parseInt(const std::string& str) const;
};

#endif
