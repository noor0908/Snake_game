#include "StatsManager.hpp"
#include <fstream>
#include <iostream>

void StatsManager::load(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        file >> totalGamesPlayed;
        file >> highestScore;
        file >> totalFoodEaten;
        file >> longestSnake;
        file >> totalPlaytime;
        file.close();
    } else {
        // Initialize default stats
        totalGamesPlayed = 0;
        highestScore = 0;
        totalFoodEaten = 0;
        longestSnake = 0;
        totalPlaytime = 0.f;
    }
}

void StatsManager::save(const std::string& filename) const {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << totalGamesPlayed << "\n";
        file << highestScore << "\n";
        file << totalFoodEaten << "\n";
        file << longestSnake << "\n";
        file << totalPlaytime << "\n";
        file.close();
    }
}

void StatsManager::recordGame(int score, int foodEaten, int maxSnakeLength, float playtimeSeconds) {
    totalGamesPlayed++;
    totalFoodEaten += foodEaten;
    totalPlaytime += playtimeSeconds;
    
    if (score > highestScore) {
        highestScore = score;
    }
    
    if (maxSnakeLength > longestSnake) {
        longestSnake = maxSnakeLength;
    }
    
    save();
}
