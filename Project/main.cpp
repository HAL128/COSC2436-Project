// COSC 2436 Project - Haruto Hirata
#include "Player.h"
#include "Sentence.h"
#include <iostream>

int main()
{
    // Sentenceクラスのテスト
    std::cout << "=== Sentence Class Test ===" << std::endl;
    std::cout << std::endl;

    // テスト用の文を作成
    Sentence sentence("Hello World");

    std::cout << "Sentence: ";
    sentence.display();
    std::cout << std::endl;

    // 文字を1つずつタイプするシミュレーション
    std::cout << "Typing simulation:" << std::endl;
    while (!sentence.isComplete())
    {
        char nextChar = sentence.getNextChar();
        std::cout << "Next char to type: '" << nextChar << "'" << std::endl;
        sentence.incrementIndex();
    }
    std::cout << std::endl;

    // 完了チェック
    std::cout << "Is complete: " << (sentence.isComplete() ? "Yes" : "No") << std::endl;
    std::cout << std::endl;

    // リセットテスト
    sentence.reset();
    std::cout << "After reset, current index: " << sentence.getCurrentIndex() << std::endl;
    std::cout << "Is complete: " << (sentence.isComplete() ? "Yes" : "No") << std::endl;

    return 0;
}
