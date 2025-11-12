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

// JSON値の文字列を整数に変換する関数
int JSONManager::parseInt(const std::string& str) const
{
    std::string trimmed = trim(str);
    return std::stoi(trimmed);
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

// JSONファイルからスコアのリストを読み込む関数
std::vector<PlayerScore> JSONManager::loadScores(const std::string& filename) const
{
    // スコアのリストを格納するベクター
    std::vector<PlayerScore> scores;

    std::ifstream file(filename);

    // ファイルが開けなかった場合、空のリストを返す
    if (!file.is_open())
    {
        return scores;
    }

    // ファイルの各行を保持
    std::string line;
    PlayerScore currentScore;
    // オブジェクトの中にいるかどうか
    bool inObject = false;

    // ファイルから各行を読み込む
    while (std::getline(file, line))
    {
        // 前後の空白を削除
        line = trim(line);

        // jsonオブジェクトの開始
        if (line.find("{") != std::string::npos)
        {
            inObject = true;
            // currentScoreを初期化
            currentScore.username = "";
            currentScore.score = 0;
        }

        if (inObject)
        {
            // "username"フィールドをチェック
            if (line.find("\"username\"") != std::string::npos)
            {
                // コロンを探す
                size_t colonPos = line.find(":");
                if (colonPos != std::string::npos)
                {
                    // コロンの後の部分文字列を取得
                    std::string value = line.substr(colonPos + 1);
                    value = trim(value);
                    if (value.back() == ',')
                    {
                        value = value.substr(0, value.length() - 1);
                    }
                    // 文字列をパースしてusernameを設定
                    currentScore.username = parseString(value);
                }
            }
            // "score"フィールドを探す
            else if (line.find("\"score\"") != std::string::npos)
            {
                // コロンの位置を探す
                size_t colonPos = line.find(":");
                if (colonPos != std::string::npos)
                {
                    // コロンの後の部分文字列を取得
                    std::string value = line.substr(colonPos + 1);
                    value = trim(value);
                    if (value.back() == ',')
                    {
                        value = value.substr(0, value.length() - 1);
                    }
                    currentScore.score = parseInt(value);
                }
            }
        }

        if (line.find("}") != std::string::npos && inObject)
        {
            inObject = false;
            if (!currentScore.username.empty())
            {
                scores.push_back(currentScore);
            }
        }
    }

    file.close();
    return scores;
}

// JSONファイルにスコアを保存する関数
void JSONManager::saveScores(const std::string& filename, const std::vector<PlayerScore>& scores) const
{
    // 書き込み用にファイルを開く
    std::ofstream file(filename);

    // ファイルが開けなかった場合
    if (!file.is_open())
    {
        std::cerr << "ERROR: Failed to open scores file for writing: " << filename << std::endl;
        return;
    }

    // JSON配列の開始ブラケットを書き込む
    file << "[\n";

    // 各スコアオブジェクトを書き込む
    for (size_t i = 0; i < scores.size(); ++i)
    {
        file << "  {\n";
        file << "    \"username\": \"" << scores[i].username << "\",\n";
        file << "    \"score\": " << scores[i].score << "\n";
        file << "  }";

        // 最後の要素でなければカンマを追加
        if (i < scores.size() - 1)
        {
            file << ",";
        }
        file << "\n";
    }

    file << "]\n";

    file.close();
}
