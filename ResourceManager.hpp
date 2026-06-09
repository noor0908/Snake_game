#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <vector>
#include "GameSettings.hpp"

class ResourceManager {
public:
    static ResourceManager& getInstance() {
        static ResourceManager instance;
        return instance;
    }

    void init(const GameSettings& settings);
    
    // Updates which music tracks are playing based on state (0 = menu/UI screens, 1 = gameplay)
    void handleMusicState(const GameSettings& settings, int musicCategory);
    
    void playSound(const std::string& name, const GameSettings& settings);
    void stopAllMusic();

    const sf::Font& getFont() const { return font; }
    sf::Font& getFont() { return font; }
    bool isFontLoaded() const { return fontLoaded; }

private:
    ResourceManager() = default;
    ~ResourceManager() = default;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    sf::Font font;
    bool fontLoaded = false;

    // Procedural sound buffers
    sf::SoundBuffer menuMusicBuffer;
    sf::SoundBuffer gameMusicBuffer;
    sf::SoundBuffer eatBuffer;
    sf::SoundBuffer clickBuffer;
    sf::SoundBuffer gameoverBuffer;
    sf::SoundBuffer unlockBuffer;

    // Sound players
    sf::Sound menuMusicSound;
    sf::Sound gameMusicSound;
    sf::Sound eatSound;
    sf::Sound clickSound;
    sf::Sound gameoverSound;
    sf::Sound unlockSound;

    // Generators
    sf::SoundBuffer generateMenuMusicBuffer();
    sf::SoundBuffer generateGameMusicBuffer();
    sf::SoundBuffer generateEatSoundBuffer();
    sf::SoundBuffer generateClickSoundBuffer();
    sf::SoundBuffer generateGameOverSoundBuffer();
    sf::SoundBuffer generateUnlockSoundBuffer();
};

#endif // RESOURCE_MANAGER_HPP
