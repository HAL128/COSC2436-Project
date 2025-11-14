#include "Game.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <conio.h>
#include <thread>

#define NOMINMAX // disable macro min/max in Windows.h
#include <windows.h>

// initialize static constants
Game::Game() : remainingTime(TOTAL_TIME), timePenalty(0), userInput("") // initialize remaining time, penalty, input string
{
    // initialize random number generator for shuffling sentences
    srand(static_cast<unsigned int>(time(nullptr)));
}

// display main menu
void Game::showMainMenu()
{
    // infinite loop - repeat until user chooses to exit
    while (true)
    {
        clearScreen();
        std::cout << "==================================" << std::endl;
        std::cout << "     Welcome to Typing Game!!     " << std::endl;
        std::cout << "==================================" << std::endl;
        std::cout << std::endl;
        std::cout << "1. Play Game" << std::endl;
        std::cout << "2. View Top 5 Rankings" << std::endl;
        std::cout << "3. Add New Sentence" << std::endl;
        std::cout << "4. Exit" << std::endl;
        std::cout << std::endl;
        std::cout << "Enter your choice (1-4): ";

        // get a single character input - no Enter needed
        char choice = _getch();

        switch (choice)
        {
        case '1':
            start();
            break;
        case '2':
            clearScreen();
            // display top 5 rankings
            displayRanking(5);
            std::cout << std::endl;
            std::cout << "Press any key to return to menu..." << std::endl;
            (void)_getch();
            break;
        case '3':
            // display add sentence screen
            addSentence();
            break;
        case '4':
            clearScreen();
            std::cout << "Goodbye!" << std::endl;
            return;
            // if any other key is pressed
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
            Sleep(1000); // wait for user to read message
            break;
        }
    }
}

// choice 3: add new sentence to JSON file
void Game::addSentence()
{
    clearScreen();
    std::cout << "==================================" << std::endl;
    std::cout << "         Add New Sentence         " << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << std::endl;

    // variable to store new sentence
    std::string newSentence;
    std::cout << "Enter a new sentence (or press Enter to cancel): " << std::endl;

    // get user input line
    std::getline(std::cin, newSentence);

    // check if input is empty
    if (newSentence.empty())
    {
        std::cout << "Cancelled." << std::endl;
        Sleep(1000); // wait for user to read message
        return;
    }

    // read existing sentences from JSON file
    std::vector<std::string> sentences = jsonManager.loadSentences("../../Project/sentences.json");

    // add the new sentence to the list
    sentences.push_back(newSentence);

    // open file to save updated sentences
    std::ofstream file("../../Project/sentences.json");

    // check if file opened successfully
    if (file.is_open())
    {
        // write first line of JSON array ([)
        file << "[\n";
        // write each sentence in JSON format
        for (size_t i = 0; i < sentences.size(); ++i)
        {
            file << "  \"" << sentences[i] << "\"";
            if (i < sentences.size() - 1)
            {
                file << ",";
            }
            file << "\n";
        }
        file << "]\n";
        // close the file
        file.close();

        std::cout << std::endl;
        std::cout << "Sentence added successfully!" << std::endl;
    }
    // if file could not be opened
    else
    {
        std::cout << std::endl;
        std::cout << "Error: Could not save the sentence." << std::endl;
    }

    std::cout << "Press any key to return to menu..." << std::endl;
    (void)_getch();
}

// choice 1: start the game
void Game::start()
{
    clearScreen();
    std::cout << "==================================" << std::endl;
    std::cout << "            Start Game            " << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << std::endl;

    // variable to store username
    std::string username;
    std::cout << "Enter your username: ";
    // get username input line
    std::getline(std::cin, username);
    player.setUsername(username);
    player.resetScore();

    // load sentence data from JSON file
    std::vector<std::string> sentences = jsonManager.loadSentences("../../Project/sentences.json");

    // check if any sentences were loaded
    if (sentences.empty())
    {
        std::cout << "Error: Could not load sentences!" << std::endl;
        std::cout << "Press any key to return to menu..." << std::endl;
        (void)_getch();
        return;
    }

    allSentences = sentences;
    availableSentences = allSentences;

    clearScreen();
    std::cout << "==================================" << std::endl;
    std::cout << "        Game Instructions         " << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "You have 2 minutes to type as many sentences as possible" << std::endl;
    std::cout << std::endl;
    std::cout << "Correct character: +" << CORRECT_CHAR_POINTS << " points" << std::endl;
    std::cout << "Complete sentence : +" << COMPLETE_SENTENCE_BONUS << " bonus points" << std::endl;
    std::cout << "Mistake: -" << MISTAKE_TIME_PENALTY << " seconds & -" << MISTAKE_SCORE_PENALTY << " points" << std::endl;
    std::cout << std::endl;
    std::cout << "Press any key to start..." << std::endl;
    (void)_getch();

    // typing game starts here
    gameLoop();

    // after game ends, show calculating screen (to avoid accidental input)
    showCalculatingScreen();

    // get current score before saving
    int currentScore = player.getScore();
    std::string currentUsername = player.getUsername();

    // load existing scores to check records
    std::vector<PlayerScore> existingScores = jsonManager.loadScores("../../Project/scores.json");

    // find user's previous best score
    int userPreviousBest = 0;
    for (const auto& score : existingScores)
    {
        if (score.username == currentUsername)
        {
            userPreviousBest = score.score;
            break;
        }
    }

    // find global best score
    int globalBest = 0;
    if (!existingScores.empty())
    {
        globalBest = existingScores[0].score; // already sorted in desc order
        for (const auto& score : existingScores)
        {
            if (score.score > globalBest) // just in case
            {
                globalBest = score.score;
            }
        }
    }

    bool isPersonalBest = (currentScore > userPreviousBest); // check personal best
    bool isGlobalBest = (currentScore > globalBest); // check global best

    // display game over and results
    clearScreen();
    std::cout << "==================================" << std::endl;
    std::cout << "            Game Over!            " << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << std::endl;
    std::cout << "Player: " << player.getUsername() << std::endl;
    std::cout << "Final Score: " << player.getScore() << " points" << std::endl;

    // save score to JSON file
    player.updateScoreDB("../../Project/scores.json", jsonManager);

    // show special message if record was broken
    if (isGlobalBest)
    {
        // display global best record message (Green color)
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        std::cout << std::endl;
        std::cout << "******************************************" << std::endl;
        std::cout << "**    === ALL-TIME RECORD BROKEN ===    **" << std::endl;
        std::cout << "**                                      **" << std::endl;
        std::cout << "**          CONGRATULATIONS !!          **" << std::endl;
        std::cout << "******************************************" << std::endl;
        SetConsoleTextAttribute(hConsole, 7);
        std::cout << std::endl;
    }
    else if (isPersonalBest)
    {
        // display personal best record message
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        std::cout << std::endl;
        std::cout << "******************************************" << std::endl;
        std::cout << "**    === PERSONAL RECORD BROKEN ===    **" << std::endl;
        std::cout << "**                                      **" << std::endl;
        std::cout << "**          CONGRATULATIONS !!          **" << std::endl;
        std::cout << "******************************************" << std::endl;
        SetConsoleTextAttribute(hConsole, 7);
        std::cout << std::endl;
    }

    bool isNewTopFiveEntry = false;
    if (isPersonalBest || isGlobalBest)
    {
        // reload scores after saving to get updated ranking
        std::vector<PlayerScore> updatedScores = jsonManager.loadScores("../../Project/scores.json");

        // check if user is in top 5
        int userRank = 0;
        for (size_t i = 0; i < updatedScores.size() && i < 5; ++i)
        {
            if (updatedScores[i].username == currentUsername)
            {
                userRank = static_cast<int>(i + 1);
                isNewTopFiveEntry = true;
                break;
            }
        }
    }

    std::cout << std::endl;
    // if user made it to top 5, highlight their name with green color
    displayRanking(5, currentUsername, isNewTopFiveEntry);

    std::cout << std::endl;
    std::cout << "Press any key to return to menu..." << std::endl;
    (void)_getch();
}

// show calculating results screen for 3 seconds (to avoid accidental input by user)
void Game::showCalculatingScreen()
{
    // hide cursor during calculation
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE; // hide cursor
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    clearScreen();
    std::cout << "==================================" << std::endl;
    std::cout << "      Calculating Results..." << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << std::endl;
    std::cout << "         Please wait..." << std::endl;
    std::cout << std::endl;

    // progress bar animation
    for (int i = 0; i <= 30; ++i)
    {
        std::cout << "\r[";
        for (int j = 0; j < 30; ++j)
        {
            if (j < i)
            {
                std::cout << "=";
            }
            else
            {
                std::cout << " ";
            }
        }
        // percent display
        std::cout << "] " << (i * 100 / 30) << "%";
        std::cout.flush();

        // clear any buffered key presses during animation
        while (_kbhit())
        {
            (void)_getch(); // discard the key
        }

        Sleep(100); // 100ms * 30 = 3 seconds
    }
    std::cout << std::endl;

    // clear any remaining buffered key presses after animation
    while (_kbhit())
    {
        (void)_getch(); // discard the key
    }

    // show cursor again
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = TRUE; // show cursor
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

// main game loop function - handles typing and screen updates
void Game::gameLoop()
{
    // hide cursor during game
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE; // hide cursor
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    // load the first sentence randomly
    loadNextSentence();

    // record start time
    startTime = std::chrono::steady_clock::now();

    // reset remaining time and penalty
    remainingTime = TOTAL_TIME;
    timePenalty = 0;

    // display player info
    clearScreen();
    std::cout << "==================================" << std::endl;
    std::cout << "   Player: " << player.getUsername() << " | Score: " << player.getScore() << std::endl;
    std::cout << "==================================" << std::endl;

    // display remaining time bar
    displayTimeBar();
    std::cout << std::endl;
    std::cout << "Type the following sentence:" << std::endl;

    // display the current sentence with progress coloring
    currentSentence.displayWithProgress(userInput);
    std::cout << std::endl;

    // blank line between sentence and input
    std::cout << std::endl;

    // display initial user input line
    std::cout << "Your input:" << std::endl;
    std::cout << userInput << std::endl;

    // record last displayed remaining time and score
    int lastDisplayedTime = remainingTime;
    int lastScore = player.getScore();

    // loop until time runs out
    while (remainingTime > 0)
    {
        // calculate remaining time based on elapsed time
        updateRemainingTime();

        // if time is up, break the loop
        if (remainingTime <= 0)
        {
            break;
        }

        // check if remaining time or score has changed -> need to update display
        bool needsUpdate = (remainingTime != lastDisplayedTime) || (player.getScore() != lastScore);

        // check if a key has been pressed
        if (_kbhit())
        {
            // get pressed key character
            char ch = _getch();

            // if ESC key pressed, break
            if (ch == 27)
            {
                break;
            }

            // check and process user input character
            processInput(ch);

            // if complete, load next sentence
            if (currentSentence.isComplete())
            {
                // add bonus points for completing the sentence
                player.addScore(COMPLETE_SENTENCE_BONUS);
                userInput.clear();

                loadNextSentence();
            }

            // display needs to be updated due to input
            needsUpdate = true;
        }

        // if time or score changed or input processed, update display
        if (needsUpdate)
        {
            // move cursor to top-left corner to overwrite entire screen
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

            // initialize coordinate struct for top-left corner
            COORD coord = { 0, 0 };

            // set cursor position to (0,0)
            SetConsoleCursorPosition(hConsole, coord);

            // display player info
            std::string line1 = "==================================";
            std::string line2 = "   Player: " + player.getUsername() + " | Score: " + std::to_string(player.getScore());

            // fill line1 to 80 characters
            line2 += std::string(80 - line2.length(), ' ');

            std::cout << line1 << std::endl;

            // display player info line
            std::cout << line2 << std::endl;
            std::cout << line1 << std::endl;
            std::cout << std::string(80, ' ') << std::endl;

            // display remaining time bar
            displayTimeBar();
            std::cout << std::string(80, ' ') << std::endl;
            std::cout << "Type the following sentence:" << std::string(60, ' ') << std::endl;

            // display current sentence with progress coloring
            currentSentence.displayWithProgress(userInput);

            // clear rest of sentence line
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            GetConsoleScreenBufferInfo(hConsole, &csbi);
            COORD clearPos = csbi.dwCursorPosition;
            DWORD written;
            FillConsoleOutputCharacter(hConsole, ' ', csbi.dwSize.X - clearPos.X, clearPos, &written);
            std::cout << std::endl;

            // blank line between sentence and input
            std::cout << std::string(80, ' ') << std::endl;

            // display "Your input:" label
            std::cout << "Your input:" << std::string(70, ' ') << std::endl;

            // clear input display line completely
            GetConsoleScreenBufferInfo(hConsole, &csbi);
            clearPos = csbi.dwCursorPosition;
            FillConsoleOutputCharacter(hConsole, ' ', csbi.dwSize.X, clearPos, &written);
            SetConsoleCursorPosition(hConsole, clearPos);

            // display user input on next line
            std::cout << userInput;

            // clear rest of line after input
            GetConsoleScreenBufferInfo(hConsole, &csbi);
            clearPos = csbi.dwCursorPosition;
            FillConsoleOutputCharacter(hConsole, ' ', csbi.dwSize.X - clearPos.X, clearPos, &written);
            std::cout << std::endl;

            lastDisplayedTime = remainingTime;
            lastScore = player.getScore();
        }
    }

    // show cursor again after game ends
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = TRUE; // show cursor
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

// remaining time as a progress bar
void Game::displayTimeBar() const
{
    int barLength = 50;
    // calculate filled length based on remaining time
    int filledLength = (remainingTime * barLength) / TOTAL_TIME;

    std::cout << "Time: [";
    for (int i = 0; i < barLength; ++i)
    {
        // check if within filled length
        if (i < filledLength)
        {
            std::cout << "=";
        }
        else
        {
            std::cout << " ";
        }
    }
    // display remaining time in seconds
    std::string timeStr = "] " + std::to_string(remainingTime) + "s";
    std::cout << timeStr << std::string(20, ' ') << std::endl;
}

// display top 5 rankings from scores.json
void Game::displayRanking(int topN, const std::string& highlightUsername, bool shouldHighlight) const
{
    // read scores from JSON file
    std::vector<PlayerScore> scores = jsonManager.loadScores("../../Project/scores.json");

    // display ranking title
    std::cout << "==================================" << std::endl;
    std::cout << "         Ranking (Top " << topN << ")" << std::endl;
    std::cout << "==================================" << std::endl;

    // check if there are no scores
    if (scores.empty())
    {
        std::cout << "No scores yet!" << std::endl;
        return;
    }

    // display top 5 scores
    int rank = 1;

    // for each score entry
    for (const auto& score : scores)
    {
        // determine if this entry should be highlighted
        bool shouldHighlightThisEntry = (shouldHighlight && score.username == highlightUsername);

        if (shouldHighlightThisEntry)
        {
            // get console handle
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        }

        // display rank, username, and score
        std::cout << rank << ". " << score.username << " - " << score.score << " points" << std::endl;

        // reset color if highlighted
        if (shouldHighlightThisEntry)
        {
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

            // reset to white
            SetConsoleTextAttribute(hConsole, 7);
        }

        rank++;
        if (rank > topN)
            break;
    }
}

// Process user input character
void Game::processInput(char inputChar)
{
    // get the expected next character from the current sentence
    char expectedChar = currentSentence.getNextChar();

    // compare input character with expected character
    if (inputChar == expectedChar)
    {
        player.addScore(CORRECT_CHAR_POINTS);
        userInput += inputChar;
        currentSentence.incrementIndex(); // move to next character
    }
    else
    {
        // apply penalties for mistake
        timePenalty += MISTAKE_TIME_PENALTY;
        player.addScore(-MISTAKE_SCORE_PENALTY);
    }
}

// load the next sentence (avoid duplicates using stack!!)
void Game::loadNextSentence()
{
    // If no available sentences, reset from used stack
    if (availableSentences.empty())
    {
        availableSentences = allSentences;
        while (!usedSentences.empty())
        {
            // pop used sentences from stack
            usedSentences.pop();
        }
    }

    // generate random index
    int randomIndex = rand() % availableSentences.size();
    // get selected sentence
    std::string selectedSentence = availableSentences[randomIndex];

    currentSentence.setText(selectedSentence);

    // add selected sentence to used stack
    usedSentences.push(selectedSentence);

    // remove selected sentence from available list
    availableSentences.erase(availableSentences.begin() + randomIndex);
}

// Clear the console screen
void Game::clearScreen() const
{
    system("cls");
}

// Get elapsed time since game start
int Game::getElapsedTime() const
{
    // get current time point
    auto currentTime = std::chrono::steady_clock::now();
    // calculate elapsed time duration in seconds
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime);
    return static_cast<int>(elapsed.count()); // return seconds
}

// update remaining time based on elapsed time and penalties
void Game::updateRemainingTime()
{
    // get elapsed time since start
    int elapsed = getElapsedTime();
    // remaining time = total time - (elapsed + penalty)
    remainingTime = TOTAL_TIME - elapsed - timePenalty;

    // if remaining time is negative, set to 0
    if (remainingTime < 0)
    {
        remainingTime = 0;
    }
}
