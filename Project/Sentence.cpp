#include "Sentence.h"
#include <iostream>

#define NOMINMAX
#include <windows.h>

// Constructor with empty sentence
Sentence::Sentence() : text(""), currentIndex(0) {}

// Constructor with specified sentence
Sentence::Sentence(const std::string& sentenceText) : text(sentenceText), currentIndex(0) {}

// Get the next character to type
char Sentence::getNextChar() const
{
    if (currentIndex < text.length())
    {
        return text[currentIndex];
    }
    return '\0';
}

// Check if typing the sentence is complete
bool Sentence::isComplete() const
{
    return currentIndex >= text.length();
}

// Display the sentence normally
void Sentence::display() const
{
    std::cout << text << std::endl;
}

// Display the sentence with progress coloring
void Sentence::displayWithProgress(const std::string& userInput) const
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    for (size_t i = 0; i < text.length(); ++i)
    {
        if (i < userInput.length())
        {
            // already typed - display in green
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            std::cout << text[i];
        }
        else if (i == userInput.length())
        {
            // current character to type - display in yellow
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            std::cout << text[i];
        }
        else
        {
            // not yet typed - display in white
            SetConsoleTextAttribute(hConsole, 7);
            std::cout << text[i];
        }
    }

    // reset color to white
    SetConsoleTextAttribute(hConsole, 7);

    // fill rest of line with spaces
    std::cout << std::string(80, ' ');
}

// Reset progress
void Sentence::reset()
{
    currentIndex = 0;
}

// Set the sentence text
void Sentence::setText(const std::string& sentenceText)
{
    text = sentenceText;
    currentIndex = 0;
}

// Get the sentence text
std::string Sentence::getText() const
{
    return text;
}

// Get the current character index
size_t Sentence::getCurrentIndex() const
{
    return currentIndex;
}

// Move to next character
void Sentence::incrementIndex()
{
    if (currentIndex < text.length())
    {
        currentIndex++;
    }
}
