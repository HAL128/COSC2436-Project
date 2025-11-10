#pragma once
#ifndef JSONMANAGER_H
#define JSONMANAGER_H

#include <string>
#include <vector>

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

private:
    // 文字列の前後の空白を削除する関数
    std::string trim(const std::string& str) const;

    // JSON文字列値から実際の文字列を抽出する関数
    // @param str: JSON文字列値
    // @return ダブルクオートを除いた文字列
    std::string parseString(const std::string& str) const;
};

#endif
