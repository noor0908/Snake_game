#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <deque>

struct Particle {
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Color color;
    float lifetime = 0.f;
    float maxLifetime = 1.f;
    float size = 3.f;
};

class ParticleSystem {
public:
    ParticleSystem() = default;

    // Emits a simple radial burst of particles at a point
    void emitBurst(const sf::Vector2f& position, int count, const sf::Color& color, float speed = 120.f);
    
    // Emits particles along the snake segments on death
    void emitDeathExplosion(const std::deque<sf::Vector2i>& snakeSegments, int tileSize);
    
    // Emits golden shower particles from the top of the screen
    void emitLevelCompleteBurst(unsigned int windowWidth, unsigned int windowHeight);

    // Emits stars falling from the top center for achievements
    void emitAchievementBurst(unsigned int windowWidth);

    void update(float dt);
    void draw(sf::RenderWindow& window);
    
    void clear();

private:
    std::vector<Particle> particles;
};

#endif // PARTICLE_SYSTEM_HPP
