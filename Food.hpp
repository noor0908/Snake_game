#ifndef FOOD_HPP
#define FOOD_HPP

#include <SFML/Graphics.hpp>
#include <string>

enum class FoodType {
    Normal,
    Bonus,
    Poison,
    SpeedBoost,
    SlowMotion
};

struct Food {
    sf::Vector2i gridPos;
    FoodType type = FoodType::Normal;
    float lifetime = 0.f; // 0 means infinite, >0 counts down to despawn
    bool active = false;

    void update(float dt) {
        if (active && lifetime > 0.f) {
            lifetime -= dt;
            if (lifetime <= 0.f) {
                active = false;
            }
        }
    }

    sf::Color getColor() const {
        switch (type) {
            case FoodType::Normal:     return sf::Color(231, 76, 60);   // Coral Red
            case FoodType::Bonus:      return sf::Color(241, 196, 15);  // Radiant Gold
            case FoodType::Poison:     return sf::Color(155, 89, 182);  // Poisonous Violet
            case FoodType::SpeedBoost: return sf::Color(52, 152, 219);  // Speed Cyan
            case FoodType::SlowMotion: return sf::Color(46, 204, 113);  // Slomo Mint Green
        }
        return sf::Color::Red;
    }

    std::string getEffectPopup() const {
        switch (type) {
            case FoodType::Normal:     return "+10";
            case FoodType::Bonus:      return "+30 GOLD!";
            case FoodType::Poison:     return "-15 POISON!";
            case FoodType::SpeedBoost: return "SPEED BOOST!";
            case FoodType::SlowMotion: return "SLOW MOTION!";
        }
        return "+10";
    }
};

#endif // FOOD_HPP
