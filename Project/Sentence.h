#pragma once
#ifndef SENTENCE_H
#define SENTENCE_H

#include <string>

// タイピングする文と進捗を管理するクラス
class Sentence
{
private:
    std::string text; // 文のテキスト
    size_t currentIndex; // 現在の文字位置

public:
    // デフォルトコンストラクタ：空の文で初期化
    Sentence();

    // 文を指定するコンストラクタ
    // @param sentenceText: タイピングする文
    Sentence(const std::string& sentenceText);

    // 次にタイプすべき文字を取得する関数
    // @return 次にタイプすべき文字
    char getNextChar() const;

    // 文のタイピングが完了したかチェックする関数
    // @return 完了していればtrue、そうでなければfalse
    bool isComplete() const;

    // 文を通常表示する関数
    void display() const;

    // 進捗をリセットする関数
    // 現在の位置を0に戻す
    void reset();

    // 文のテキストを設定する関数
    // @param sentenceText: 新しい文
    void setText(const std::string& sentenceText);

    // 文のテキストを取得する関数
    // @return 現在の文
    std::string getText() const;

    // 現在の文字位置を取得する関数
    // @return 現在の位置
    size_t getCurrentIndex() const;

    // 次の文字に移動する関数
    void incrementIndex();
};

#endif
