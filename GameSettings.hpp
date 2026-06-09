#ifndef GAME_SETTINGS_HPP
#define GAME_SETTINGS_HPP

#include <string>
#include <fstream>
#include <iostream>

enum class SpeedMode { Slow, Normal, Fast, Extreme };
enum class DifficultyMode { Easy, Medium, Hard };

struct GameSettings {
    bool musicEnabled = true;
    bool sfxEnabled = true;
    SpeedMode speedMode = SpeedMode::Normal;
    DifficultyMode difficultyMode = DifficultyMode::Hard; // Default to Hard (classic killing borders)

    float getSpeedDelay() const {
        switch (speedMode) {
            case SpeedMode::Slow:    return 0.15f;
            case SpeedMode::Normal:  return 0.10f;
            case SpeedMode::Fast:    return 0.07f;
            case SpeedMode::Extreme: return 0.04f;
        }
        return 0.10f;
    }

    std::string getSpeedString() const {
        switch (speedMode) {
            case SpeedMode::Slow:    return "Slow";
            case SpeedMode::Normal:  return "Normal";
            case SpeedMode::Fast:    return "Fast";
            case SpeedMode::Extreme: return "Extreme";
        }
        return "Normal";
    }

    std::string getDifficultyString() const {
        switch (difficultyMode) {
            case DifficultyMode::Easy:   return "Easy (Wrap, No Walls)";
            case DifficultyMode::Medium: return "Medium (Wrap + Walls)";
            case DifficultyMode::Hard:   return "Hard (Classic Death)";
        }
        return "Hard";
    }

    void cycleSpeed() {
        switch (speedMode) {
            case SpeedMode::Slow:    speedMode = SpeedMode::Normal; break;
            case SpeedMode::Normal:  speedMode = SpeedMode::Fast; break;
            case SpeedMode::Fast:    speedMode = SpeedMode::Extreme; break;
            case SpeedMode::Extreme: speedMode = SpeedMode::Slow; break;
        }
    }

    void cycleDifficulty() {
        switch (difficultyMode) {
            case DifficultyMode::Easy:   difficultyMode = DifficultyMode::Medium; break;
            case DifficultyMode::Medium: difficultyMode = DifficultyMode::Hard; break;
            case DifficultyMode::Hard:   difficultyMode = DifficultyMode::Easy; break;
        }
    }

    void saveToFile(const std::string& filename = "settings.txt") const {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << musicEnabled << "\n";
            file << sfxEnabled << "\n";
            file << static_cast<int>(speedMode) << "\n";
            file << static_cast<int>(difficultyMode) << "\n";
            file.close();
        }
    }

    void loadFromFile(const std::string& filename = "settings.txt") {
        std::ifstream file(filename);
        if (file.is_open()) {
            int speedVal = 1;
            int diffVal = 2; // Default to Hard
            file >> musicEnabled;
            file >> sfxEnabled;
            file >> speedVal;
            if (file >> diffVal) {
                if (diffVal >= 0 && diffVal <= 2) {
                    difficultyMode = static_cast<DifficultyMode>(diffVal);
                }
            }
            if (speedVal >= 0 && speedVal <= 3) {
                speedMode = static_cast<SpeedMode>(speedVal);
            }
            file.close();
        }
    }
};

#endif // GAME_SETTINGS_HPP
