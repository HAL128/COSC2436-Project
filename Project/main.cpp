// COSC 2436 Project - Haruto Hirata
#include "JSONManager.h"
#include <iostream>

int main()
{
    // JSONManagerクラスのテスト
    std::cout << "=== JSONManager Class Test ===" << std::endl;
    std::cout << std::endl;

    // JSONManagerを作成
    JSONManager jsonManager;

    // sentencesファイルから文のリストを読み込む
    std::vector<std::string> sentences = jsonManager.loadSentences("../../Project/sentences.json");

    // 読み込んだ文の数を表示
    std::cout << "Loaded " << sentences.size() << " sentences:" << std::endl;
    std::cout << std::endl;

    // 各文を表示
    for (size_t i = 0; i < sentences.size(); ++i)
    {
        std::cout << (i + 1) << ". " << sentences[i] << std::endl;
    }

    return 0;
}
