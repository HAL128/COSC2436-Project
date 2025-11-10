#include "JSONManager.h"
#include <fstream>
#include <iostream>

// コンストラクタ
JSONManager::JSONManager() {}

// 文字列の前後の空白を削除する関数
std::string JSONManager::trim(const std::string& str) const
{
    // 最初の非空白文字を探す
    size_t first = str.find_first_not_of(" \t\n\r");

    // 文字列が全て空白の場合
    if (first == std::string::npos)
        return "";

    // 最後の非空白文字を探す
    size_t last = str.find_last_not_of(" \t\n\r");

    // 最初から最後までの部分文字列を返す
    return str.substr(first, (last - first + 1));
}

// JSON文字列値から実際の文字列を抽出する関数
std::string JSONManager::parseString(const std::string& str) const
{
    std::string trimmed = trim(str);
    // 前後のダブルクオートを除去
    if (trimmed.front() == '"' && trimmed.back() == '"')
    {
        return trimmed.substr(1, trimmed.length() - 2);
    }
    return trimmed;
}

// JSONファイルから文のリストを読み込む関数
std::vector<std::string> JSONManager::loadSentences(const std::string& filename) const
{
    // 文のリストを格納するベクター
    std::vector<std::string> sentenceList;

    // ファイルを開く
    std::ifstream file(filename);

    // ファイルが正常に開けたかチェック
    if (!file.is_open())
    {
        std::cerr << "ERROR: Failed to open sentences file: " << filename << std::endl;
        return sentenceList;
    }

    // ファイルを1行ずつ読み込む
    std::string line;
    // 配列の中にいるかどうかのフラグ
    bool inArray = false;

    // 各行を読み込む
    while (std::getline(file, line))
    {
        // 前後の空白を削除
        line = trim(line);

        // json配列の開始
        if (line.find("[") != std::string::npos)
        {
            inArray = true;
            continue;
        }

        // json配列の終了
        if (line.find("]") != std::string::npos)
        {
            inArray = false;
            break;
        }

        // ダブルクオートで囲まれた文字列を処理
        if (inArray && line.find("\"") != std::string::npos)
        {
            // 最初と最後のダブルクオートの位置を見つける
            size_t start = line.find("\"");
            size_t end = line.rfind("\"");

            if (start != std::string::npos && end != std::string::npos && start < end)
            {
                // クオート間の文字列を抽出
                std::string sentence = line.substr(start + 1, end - start - 1);
                if (!sentence.empty())
                {
                    sentenceList.push_back(sentence);
                }
            }
        }
    }

    file.close();

    return sentenceList;
}
