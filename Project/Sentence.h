#pragma once
#ifndef SENTENCE_H
#define SENTENCE_H

#include <string>

// Class to manage typing sentences and progress
class Sentence
{
private:
    std::string text;
    size_t currentIndex;

public:
    // Constructor with empty sentence
    Sentence();

    // Constructor with specified sentence
    // @param sentenceText: sentence to type
    Sentence(const std::string& sentenceText);

    // Get the next character to type
    // @return next character to type
    char getNextChar() const;

    // Check if typing the sentence is complete
    // @return true if complete, false otherwise
    bool isComplete() const;

    // Display the sentence normally
    void display() const;

    // Display the sentence with progress coloring
    // @param userInput: string of characters already typed by the player
    void displayWithProgress(const std::string& userInput) const;

    // Reset progress
    // Set current index back to 0
    void reset();

    // Set the sentence text
    // @param sentenceText: new sentence to set
    void setText(const std::string& sentenceText);

    // Get the sentence text
    // @return current sentence text
    std::string getText() const;

    // Get the current character index
    // @return current index
    size_t getCurrentIndex() const;

    // Move to next character
    void incrementIndex();
};

#endif
