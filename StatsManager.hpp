#ifndef STATS_MANAGER_HPP
#define STATS_MANAGER_HPP

#include <string>

class StatsManager {
public:
    static StatsManager& getInstance() {
        static StatsManager instance;
        return instance;
    }

    void load(const std::string& filename = "stats.txt");
    void save(const std::string& filename = "stats.txt") const;

    void recordGame(int score, int foodEaten, int maxSnakeLength, float playtimeSeconds);

    int getTotalGamesPlayed() const { return totalGamesPlayed; }
    int getHighestScore() const { return highestScore; }
    int getTotalFoodEaten() const { return totalFoodEaten; }
    int getLongestSnake() const { return longestSnake; }
    float getTotalPlaytime() const { return totalPlaytime; }

private:
    StatsManager() = default;
    ~StatsManager() = default;
    StatsManager(const StatsManager&) = delete;
    StatsManager& operator=(const StatsManager&) = delete;

    int totalGamesPlayed = 0;
    int highestScore = 0;
    int totalFoodEaten = 0;
    int longestSnake = 0;
    float totalPlaytime = 0.f; // in seconds
};

#endif // STATS_MANAGER_HPP
