#include "ParticleSystem.hpp"
#include <cmath>
#include <cstdlib>

void ParticleSystem::emitBurst(const sf::Vector2f& position, int count, const sf::Color& color, float speed) {
    for (int i = 0; i < count; ++i) {
        float angle = static_cast<float>(rand() % 360) * 3.14159f / 180.f;
        float actualSpeed = (static_cast<float>(rand() % 100) / 100.f) * speed + 30.f;
        
        Particle p;
        p.position = position;
        p.velocity = sf::Vector2f(cos(angle) * actualSpeed, sin(angle) * actualSpeed);
        p.color = color;
        p.maxLifetime = (static_cast<float>(rand() % 50) / 100.f) + 0.3f;
        p.lifetime = p.maxLifetime;
        p.size = static_cast<float>(rand() % 3 + 2); // 2 to 4 pixels

        particles.push_back(p);
    }
}

void ParticleSystem::emitDeathExplosion(const std::deque<sf::Vector2i>& snakeSegments, int tileSize) {
    for (const auto& segment : snakeSegments) {
        sf::Vector2f pos(
            static_cast<float>(segment.x * tileSize + tileSize / 2),
            static_cast<float>(segment.y * tileSize + tileSize / 2)
        );
        // Mix of snake green and orange colors
        sf::Color colors[] = {
            sf::Color(46, 204, 113), // Bright green
            sf::Color(39, 174, 96),  // Dark green
            sf::Color(230, 126, 34)  // Orange
        };
        sf::Color selectedColor = colors[rand() % 3];
        emitBurst(pos, 6, selectedColor, 140.f);
    }
}

void ParticleSystem::emitLevelCompleteBurst(unsigned int windowWidth, unsigned int windowHeight) {
    // Generate fireworks/sparkles from random positions in the screen
    for (int k = 0; k < 5; ++k) {
        sf::Vector2f pos(
            static_cast<float>(rand() % (windowWidth - 100) + 50),
            static_cast<float>(rand() % (windowHeight - 200) + 100)
        );
        sf::Color colors[] = {
            sf::Color(241, 196, 15),  // Gold
            sf::Color(52, 152, 219),  // Blue
            sf::Color(155, 89, 182),  // Purple
            sf::Color(231, 76, 60),   // Red
            sf::Color(46, 204, 113)   // Green
        };
        emitBurst(pos, 25, colors[rand() % 5], 160.f);
    }
}

void ParticleSystem::emitAchievementBurst(unsigned int windowWidth) {
    // Shower particles falling from top center
    sf::Vector2f pos(static_cast<float>(windowWidth / 2), 40.f);
    for (int i = 0; i < 40; ++i) {
        float angle = static_cast<float>(rand() % 120 + 30) * 3.14159f / 180.f; // downwards arc
        float speed = static_cast<float>(rand() % 100 + 80);
        
        Particle p;
        p.position = pos;
        p.velocity = sf::Vector2f(cos(angle) * speed, sin(angle) * speed);
        p.color = sf::Color(241, 196, 15); // Golden stars
        p.maxLifetime = (static_cast<float>(rand() % 100) / 100.f) + 1.0f; // long lifetime
        p.lifetime = p.maxLifetime;
        p.size = static_cast<float>(rand() % 4 + 3);
        
        particles.push_back(p);
    }
}

void ParticleSystem::update(float dt) {
    for (auto& p : particles) {
        // Apply gravity if they are longer-lived (e.g. sparks falling)
        if (p.maxLifetime > 0.8f) {
            p.velocity.y += 120.f * dt; // gravitational pull
        }
        
        p.position += p.velocity * dt;
        p.lifetime -= dt;
        
        // Smoothly fade out opacity
        float ratio = p.lifetime / p.maxLifetime;
        if (ratio < 0.f) ratio = 0.f;
        p.color.a = static_cast<sf::Uint8>(ratio * 255.f);
    }

    // Clean up expired particles
    particles.erase(
        std::remove_if(particles.begin(), particles.end(), [](const Particle& p) { return p.lifetime <= 0.f; }),
        particles.end()
    );
}

void ParticleSystem::draw(sf::RenderWindow& window) {
    for (const auto& p : particles) {
        sf::RectangleShape shape(sf::Vector2f(p.size, p.size));
        shape.setPosition(p.position);
        shape.setFillColor(p.color);
        window.draw(shape);
    }
}

void ParticleSystem::clear() {
    particles.clear();
}
