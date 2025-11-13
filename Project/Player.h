#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include "JSONManager.h"

// Class to manage player (user) information
class Player
{
private:
    std::string username; // username
    int score; // current score

public:
    // Constructor with empty name and score 0
    Player();

    // Set constructor with specified name
    Player(const std::string& name);

    // Add points to score
    void addScore(int points);

    // Set username
    void setUsername(const std::string& name);

    // Get username
    std::string getUsername() const;

    // Get current score
    int getScore() const;

    // Reset score to 0
    void resetScore();

    // Update score database and ranking
    void updateScoreDB(const std::string& filename, JSONManager& jsonManager);
};

#endif
