#pragma once
#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Sentence.h"
#include "JSONManager.h"
#include <string>
#include <vector>
#include <stack>
#include <chrono>

// Game Class to manage the overall typing game
class Game
{
private:
    // player object to hold player info: name and score
    Player player;

    // current sentence to type
    Sentence currentSentence;

    // JSON manager for file operations
    JSONManager jsonManager;

    // all sentences loaded from JSON file
    std::vector<std::string> allSentences;
    // sentences available for selection
    std::vector<std::string> availableSentences;
    // stack to track used sentences to avoid duplicates
    std::stack<std::string> usedSentences;

    // game state variables
    int remainingTime; // remaining time in seconds
    int timePenalty; // accumulated time penalty in seconds
    const int TOTAL_TIME = 120; // total game time in seconds
    const int CORRECT_CHAR_POINTS = 10; // points per correct character
    const int COMPLETE_SENTENCE_BONUS = 20; // bonus points for completing a sentence
    const int MISTAKE_TIME_PENALTY = 2; // seconds penalty per mistake
    const int MISTAKE_SCORE_PENALTY = 5; // score penalty per mistake

    // user input string to track typed characters
    std::string userInput;

    // start time point for elapsed time calculation
    std::chrono::steady_clock::time_point startTime;

public:
    // Constructor
    Game();

    // display main menu function
    void showMainMenu();

    // start game function - initializes and starts the typing game
    void start();

    // main game loop function - handles typing and screen updates
    void gameLoop();

    // display remaining time bar function
    void displayTimeBar() const;

    // display top 5 rankings from scores.json
    void displayRanking(int topN = 10, const std::string& highlightUsername = "", bool shouldHighlight = false) const;

    // process user input function - compares input with expected character
    void processInput(char inputChar);

    // load next sentence randomlly (avoid duplicates using stack!!)
    void loadNextSentence();

    // clear the console screen
    void clearScreen() const;

    // elapsed time in seconds
    int getElapsedTime() const;

    // update remaining time
    void updateRemainingTime();

    // save new sentence to JSON file
    void addSentence();

    // display calculating screen function for 3 seconds animation
    void showCalculatingScreen();
};

#endif
