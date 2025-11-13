#include "JSONManager.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <cstdlib>

#define NOMINMAX // disable macro min/max in Windows.h so we can use std::min/std::max
#include <direct.h>

JSONManager::JSONManager() {}

// Trim to remove leading/trailing whitespace
std::string JSONManager::trim(const std::string& str) const
{
    // find first non-whitespace character
    size_t first = str.find_first_not_of(" \t\n\r");

    // check if the string is all whitespace
    if (first == std::string::npos)
        return "";

    // find last non-whitespace character
    size_t last = str.find_last_not_of(" \t\n\r");

    // return the substring from first to last
    return str.substr(first, (last - first + 1));
}

// extract string value from JSON string
std::string JSONManager::parseString(const std::string& str) const
{
    std::string trimmed = trim(str);
    if (trimmed.front() == '"' && trimmed.back() == '"')
    {
        return trimmed.substr(1, trimmed.length() - 2);
    }
    return trimmed;
}

// convert string to integer of JSON value
int JSONManager::parseInt(const std::string& str) const
{
    std::string trimmed = trim(str);
    return std::stoi(trimmed);
}

// read sentences from JSON file
std::vector<std::string> JSONManager::loadSentences(const std::string& filename) const
{
    std::vector<std::string> sentenceList;

    std::ifstream file(filename);

    // check if file opened successfully
    if (!file.is_open())
    {
        std::cerr << "ERROR: Failed to open sentences file: " << filename << std::endl;
        return sentenceList;
    }

    // read file line by line
    std::string line;
    // unless we are inside the array
    bool inArray = false;

    // read each line from the file
    while (std::getline(file, line))
    {
        // delete leading/trailing whitespace
        line = trim(line);

        // json array start
        if (line.find("[") != std::string::npos)
        {
            // set inArray to true
            inArray = true;
            continue;
        }

        // json array end
        if (line.find("]") != std::string::npos)
        {
            // set inArray to false
            inArray = false;
            break;
        }

        // manage double-quoted strings
        if (inArray && line.find("\"") != std::string::npos)
        {
            size_t start = line.find("\"");
            size_t end = line.rfind("\"");
            if (start != std::string::npos && end != std::string::npos && start < end)
            {
                std::string sentence = line.substr(start + 1, end - start - 1);
                if (!sentence.empty())
                {
                    sentenceList.push_back(sentence);
                }
            }
        }
    }

    file.close();

    return sentenceList;
}

// read scores from JSON file
std::vector<PlayerScore> JSONManager::loadScores(const std::string& filename) const
{
    // vector to hold list of scores
    std::vector<PlayerScore> scores;

    std::ifstream file(filename);

    // if file failed to open, return empty list
    if (!file.is_open())
    {
        return scores;
    }

    // hold each line of the file
    std::string line;
    PlayerScore currentScore;
    // unless we are inside the array
    bool inObject = false;

    // read each line from the file
    while (std::getline(file, line))
    {
        // delete leading/trailing whitespace
        line = trim(line);

        // json array start
        if (line.find("{") != std::string::npos)
        {
            // set inObject to true
            inObject = true;
            // initialize currentScore
            currentScore.username = "";
            // set score 0
            currentScore.score = 0;
        }

        if (inObject)
        {
            // check for "username" field
            if (line.find("\"username\"") != std::string::npos)
            {
                // find :
                size_t colonPos = line.find(":");
                if (colonPos != std::string::npos)
                {
                    // get substring after colon
                    std::string value = line.substr(colonPos + 1);
                    value = trim(value);
                    if (value.back() == ',')
                    {
                        value = value.substr(0, value.length() - 1);
                    }
                    // set username after parsing string
                    currentScore.username = parseString(value);
                }
            }
            // find "score" field
            else if (line.find("\"score\"") != std::string::npos)
            {
                // find the position of colon
                size_t colonPos = line.find(":");
                if (colonPos != std::string::npos)
                {
                    // get substring after colon
                    std::string value = line.substr(colonPos + 1);
                    value = trim(value);
                    if (value.back() == ',')
                    {
                        value = value.substr(0, value.length() - 1);
                    }
                    currentScore.score = parseInt(value);
                }
            }
        }

        if (line.find("}") != std::string::npos && inObject)
        {
            inObject = false;
            if (!currentScore.username.empty())
            {
                scores.push_back(currentScore);
            }
        }
    }

    file.close();
    return scores;
}

// save scores to JSON file
void JSONManager::saveScores(const std::string& filename, const std::vector<PlayerScore>& scores) const
{
    // open file for writing
    std::ofstream file(filename);

    // if file failed to open
    if (!file.is_open())
    {
        std::cerr << "ERROR: Failed to open scores file for writing: " << filename << std::endl;
        return;
    }

    // write opening bracket for JSON array
    file << "[\n";

    // write each score object
    for (size_t i = 0; i < scores.size(); ++i)
    {
        file << "  {\n";
        file << "    \"username\": \"" << scores[i].username << "\",\n";
        file << "    \"score\": " << scores[i].score << "\n";
        file << "  }";

        // add comma if not last element
        if (i < scores.size() - 1)
        {
            file << ",";
        }
        file << "\n";
    }

    file << "]\n";

    file.close();
}
