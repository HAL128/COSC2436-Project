#include "Player.h"
#include <algorithm>
#include <iostream>

// Constructor with empty name and score 0
Player::Player() : username(""), score(0) {}

// Constructor with specified name
Player::Player(const std::string& name) : username(name), score(0) {}

// Add points to score
void Player::addScore(int points)
{
    score += points;
    // Ensure score doesn't go negative
    if (score < 0)
    {
        score = 0;
    }
}

// Set username
void Player::setUsername(const std::string& name)
{
    username = name;
}

// Get username
std::string Player::getUsername() const
{
    return username;
}

// Get current score
int Player::getScore() const
{
    return score;
}

// Reset score to 0
void Player::resetScore()
{
    score = 0;
}

// Update score database and ranking
// @param filename: path to JSON score file
// @param jsonManager: JSONManager instance for file operations
void Player::updateScoreDB(const std::string& filename, JSONManager& jsonManager)
{
    // Load existing score data
    std::vector<PlayerScore> scores = jsonManager.loadScores(filename);

    // Check if the same username already exists
    bool found = false;
    for (auto& playerScore : scores)
    {
        if (playerScore.username == username)
        {
            // Update only if current score is higher than existing score
            if (score > playerScore.score)
            {
                playerScore.score = score;
                std::cout << "New high score for " << username << "!" << std::endl;
            }
            found = true;
            break;
        }
    }

    // If new user, add to list
    if (!found)
    {
        PlayerScore newScore;
        newScore.username = username;
        newScore.score = score;
        scores.push_back(newScore);
    }

    // Sort scores in desc (create ranking)
    std::sort(scores.begin(), scores.end(), [](const PlayerScore& a, const PlayerScore& b)
        { return a.score > b.score; });

    // Save updated score data to file
    jsonManager.saveScores(filename, scores);
}
