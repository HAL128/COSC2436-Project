#include "Sentence.h"
#include <iostream>

// デフォルトコンストラクタ：空の文で初期化
Sentence::Sentence() : text(""), currentIndex(0) {}

// 文を指定するコンストラクタ
Sentence::Sentence(const std::string& sentenceText) : text(sentenceText), currentIndex(0) {}

// 次にタイプすべき文字を取得する関数
char Sentence::getNextChar() const
{
    // 現在の位置が文の長さより小さい場合
    if (currentIndex < text.length())
    {
        return text[currentIndex];
    }
    // 文の終わりを超えている場合はヌル文字を返す
    return '\0';
}

// 文のタイピングが完了したかチェックする関数
bool Sentence::isComplete() const
{
    return currentIndex >= text.length();
}

// 文を通常表示する関数
void Sentence::display() const
{
    std::cout << text << std::endl;
}

// 進捗をリセットする関数
void Sentence::reset()
{
    currentIndex = 0;
}

// 文のテキストを設定する関数
void Sentence::setText(const std::string& sentenceText)
{
    text = sentenceText;
    currentIndex = 0; // 新しい文を設定したら位置もリセット
}

// 文のテキストを取得する関数
std::string Sentence::getText() const
{
    return text;
}

// 現在の文字位置を取得する関数
size_t Sentence::getCurrentIndex() const
{
    return currentIndex;
}

// 次の文字に移動する関数
void Sentence::incrementIndex()
{
    // 文の長さを超えない範囲で位置を進める
    if (currentIndex < text.length())
    {
        currentIndex++;
    }
}
