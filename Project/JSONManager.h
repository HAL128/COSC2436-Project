#pragma once
#ifndef JSONMANAGER_H
#define JSONMANAGER_H

#include <string>
#include <vector>

struct PlayerScore
{
    std::string username; // username
    int score; // score
};

class JSONManager
{
public:
    JSONManager();

    // Read sentences from JSON file
    // @param filename: path to JSON file
    // @return list of sentences
    std::vector<std::string> loadSentences(const std::string& filename) const;

    // Read scores from JSON file
    // @param filename: path to JSON file
    // @return list of PlayerScore structs
    std::vector<PlayerScore> loadScores(const std::string& filename) const;

    // Save scores to JSON file
    // @param filename: path to JSON file
    // @param scores: list of PlayerScore structs to save
    void saveScores(const std::string& filename, const std::vector<PlayerScore>& scores) const;

private:
    // delete copy constructor and assignment operator to prevent copying
    std::string trim(const std::string& str) const;

    // extract string value from JSON string
    // @param str: JSON string value
    // @return parsed string without quotes
    std::string parseString(const std::string& str) const;

    // convert string to integer of JSON value
    // @param str: JSON integer value
    // @return parsed integer
    int parseInt(const std::string& str) const;
};

#endif
