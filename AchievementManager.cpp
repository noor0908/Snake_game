#include "AchievementManager.hpp"
#include "ResourceManager.hpp"
#include <fstream>
#include <iostream>

void AchievementManager::init() {
    achievements.clear();
    
    // Define the 6 standard achievements
    achievements.push_back({"first_apple", "First Bite", "Eat your very first food segment", false});
    achievements.push_back({"score_100", "Century Club", "Reach a score of 100 in a game", false});
    achievements.push_back({"score_500", "Grand Master", "Reach a score of 500 in a game", false});
    achievements.push_back({"level_1", "First Step", "Complete Level 1", false});
    achievements.push_back({"level_all", "Champion", "Complete all 5 predefined stages", false});
    achievements.push_back({"survive_5", "Survivor", "Stay alive for 5 minutes in a single game", false});

    load();
}

void AchievementManager::load(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string id;
        int status;
        while (file >> id >> status) {
            for (auto& ach : achievements) {
                if (ach.id == id) {
                    ach.unlocked = (status != 0);
                    break;
                }
            }
        }
        file.close();
    }
}

void AchievementManager::save(const std::string& filename) const {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto& ach : achievements) {
            file << ach.id << " " << (ach.unlocked ? 1 : 0) << "\n";
        }
        file.close();
    }
}

void AchievementManager::unlock(const std::string& id, const GameSettings& settings) {
    for (auto& ach : achievements) {
        if (ach.id == id) {
            if (!ach.unlocked) {
                ach.unlocked = true;
                save();

                // Play procedural chime
                ResourceManager::getInstance().playSound("unlock", settings);

                // Queue toast notification
                AchievementToast toast;
                toast.title = ach.title;
                activeToasts.push_back(toast);
            }
            break;
        }
    }
}

bool AchievementManager::isUnlocked(const std::string& id) const {
    for (const auto& ach : achievements) {
        if (ach.id == id) {
            return ach.unlocked;
        }
    }
    return false;
}

void AchievementManager::update(float dt) {
    for (auto& toast : activeToasts) {
        if (toast.lifetime > 0.f) {
            // Slide down
            if (toast.currentY < toast.targetY) {
                toast.currentY += 300.f * dt;
                if (toast.currentY > toast.targetY) toast.currentY = toast.targetY;
            } else {
                toast.lifetime -= dt;
            }
        } else {
            // Slide up
            toast.currentY -= 300.f * dt;
            if (toast.currentY <= -80.f) {
                toast.isDone = true;
            }
        }
    }

    // Remove finished toasts
    activeToasts.erase(
        std::remove_if(activeToasts.begin(), activeToasts.end(), [](const AchievementToast& t) { return t.isDone; }),
        activeToasts.end()
    );
}

void AchievementManager::draw(sf::RenderWindow& window) {
    if (activeToasts.empty()) return;
    const auto& font = ResourceManager::getInstance().getFont();

    for (const auto& toast : activeToasts) {
        float x = (window.getSize().x / 2.f) - 170.f;
        float y = toast.currentY;

        // Draw Panel Background
        sf::RectangleShape panel(sf::Vector2f(340.f, 60.f));
        panel.setPosition(x, y);
        panel.setFillColor(sf::Color(25, 30, 45, 235)); // Sleek translucent dark blue
        panel.setOutlineColor(sf::Color(241, 196, 15)); // Golden border
        panel.setOutlineThickness(2.f);

        // Gold Star decoration
        sf::ConvexShape star(5);
        // A simple star polygon
        star.setPointCount(10);
        float r1 = 6.f, r2 = 12.f;
        sf::Vector2f center(x + 25.f, y + 30.f);
        for (int i = 0; i < 10; ++i) {
            float angle = i * M_PI / 5.f - M_PI / 2.f;
            float r = (i % 2 == 0) ? r2 : r1;
            star.setPoint(i, sf::Vector2f(center.x + r * cos(angle), center.y + r * sin(angle)));
        }
        star.setFillColor(sf::Color(241, 196, 15));

        // Text labels
        sf::Text header;
        header.setFont(font);
        header.setString("ACHIEVEMENT UNLOCKED!");
        header.setCharacterSize(12);
        header.setFillColor(sf::Color(241, 196, 15)); // Gold
        header.setPosition(x + 50.f, y + 12.f);

        sf::Text title;
        title.setFont(font);
        title.setString(toast.title);
        title.setCharacterSize(16);
        title.setFillColor(sf::Color::White);
        title.setPosition(x + 50.f, y + 28.f);

        window.draw(panel);
        window.draw(star);
        window.draw(header);
        window.draw(title);
    }
}
