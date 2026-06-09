#ifndef ACHIEVEMENT_MANAGER_HPP
#define ACHIEVEMENT_MANAGER_HPP

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "GameSettings.hpp"

struct Achievement {
    std::string id;
    std::string title;
    std::string description;
    bool unlocked = false;
};

struct AchievementToast {
    std::string title;
    float currentY = -80.f;
    float targetY = 30.f;
    float lifetime = 3.5f;
    bool isDone = false;
};

class AchievementManager {
public:
    static AchievementManager& getInstance() {
        static AchievementManager instance;
        return instance;
    }

    void init();
    void load(const std::string& filename = "achievements.txt");
    void save(const std::string& filename = "achievements.txt") const;

    void unlock(const std::string& id, const GameSettings& settings);
    bool isUnlocked(const std::string& id) const;

    void update(float dt);
    void draw(sf::RenderWindow& window);

    const std::vector<Achievement>& getAchievements() const { return achievements; }

private:
    AchievementManager() = default;
    ~AchievementManager() = default;
    AchievementManager(const AchievementManager&) = delete;
    AchievementManager& operator=(const AchievementManager&) = delete;

    std::vector<Achievement> achievements;
    std::vector<AchievementToast> activeToasts;
};

#endif // ACHIEVEMENT_MANAGER_HPP
