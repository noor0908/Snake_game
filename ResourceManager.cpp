#include "ResourceManager.hpp"
#include "EmbeddedAssets.hpp"
#include <cmath>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void ResourceManager::init(const GameSettings& settings) {
    // Load font from embedded memory array
    fontLoaded = font.loadFromMemory(ProggyTiny_ttf, ProggyTiny_ttf_len);
    if (!fontLoaded) {
        std::cerr << "ERROR: Failed to load embedded font from memory!" << std::endl;
    }

    // Generate procedural sound buffers
    menuMusicBuffer = generateMenuMusicBuffer();
    menuMusicSound.setBuffer(menuMusicBuffer);
    menuMusicSound.setLoop(true);
    hasMenuMusic = true;

    gameMusicBuffer = generateGameMusicBuffer();
    gameMusicSound.setBuffer(gameMusicBuffer);
    gameMusicSound.setLoop(true);
    hasGameMusic = true;

    eatBuffer = generateEatSoundBuffer();
    eatSound.setBuffer(eatBuffer);
    hasEatSound = true;

    clickBuffer = generateClickSoundBuffer();
    clickSound.setBuffer(clickBuffer);
    hasClickSound = true;

    gameoverBuffer = generateGameOverSoundBuffer();
    gameoverSound.setBuffer(gameoverBuffer);
    hasGameoverSound = true;

    unlockBuffer = generateUnlockSoundBuffer();
    unlockSound.setBuffer(unlockBuffer);
    hasUnlockSound = true;
}

void ResourceManager::handleMusicState(const GameSettings& settings, int musicCategory) {
    if (settings.musicEnabled) {
        if (musicCategory == 0) { // UI, Menus
            if (gameMusicSound.getStatus() == sf::Sound::Playing) {
                gameMusicSound.stop();
            }
            if (menuMusicSound.getStatus() != sf::Sound::Playing) {
                menuMusicSound.play();
            }
        } else if (musicCategory == 1) { // Gameplay
            if (menuMusicSound.getStatus() == sf::Sound::Playing) {
                menuMusicSound.stop();
            }
            if (gameMusicSound.getStatus() != sf::Sound::Playing) {
                gameMusicSound.play();
            }
        }
    } else {
        stopAllMusic();
    }
}

void ResourceManager::playSound(const std::string& name, const GameSettings& settings) {
    if (!settings.sfxEnabled) return;

    if (name == "eat" && hasEatSound) {
        eatSound.play();
    } else if (name == "click" && hasClickSound) {
        clickSound.play();
    } else if (name == "gameover" && hasGameoverSound) {
        gameoverSound.play();
    } else if (name == "unlock" && hasUnlockSound) {
        unlockSound.play();
    }
}

void ResourceManager::stopAllMusic() {
    if (menuMusicSound.getStatus() == sf::Sound::Playing) {
        menuMusicSound.stop();
    }
    if (gameMusicSound.getStatus() == sf::Sound::Playing) {
        gameMusicSound.stop();
    }
}

// Procedural generators
sf::SoundBuffer ResourceManager::generateMenuMusicBuffer() {
    std::vector<sf::Int16> samples;
    unsigned int sampleRate = 22050;
    float duration = 6.0f;
    unsigned int sampleCount = static_cast<unsigned int>(duration * sampleRate);
    samples.reserve(sampleCount);

    float chords[4][3] = {
        { 220.0f, 261.63f, 329.63f }, // Am
        { 174.61f, 220.0f, 261.63f }, // F
        { 261.63f, 329.63f, 392.00f }, // C
        { 196.00f, 246.94f, 293.66f }  // G
    };

    for (unsigned int i = 0; i < sampleCount; ++i) {
        double t = static_cast<double>(i) / sampleRate;
        int chordIndex = static_cast<int>(t / 1.5) % 4;
        
        double bassFreq = chords[chordIndex][0] / 2.0;
        double bass = sin(2.0 * M_PI * bassFreq * t);

        int noteIndex = static_cast<int>(t / 0.25) % 3;
        double arpFreq = chords[chordIndex][noteIndex];
        double arpSin = sin(2.0 * M_PI * arpFreq * t);
        double arp = (arpSin > 0.0) ? 1.0 : -1.0;
        double env = exp(-8.0 * fmod(t, 0.25));

        double mixed = 0.3 * bass + 0.1 * arp * env;
        
        if (mixed > 1.0) mixed = 1.0;
        if (mixed < -1.0) mixed = -1.0;
        samples.push_back(static_cast<sf::Int16>(mixed * 32767));
    }

    sf::SoundBuffer buffer;
    buffer.loadFromSamples(&samples[0], samples.size(), 1, sampleRate);
    return buffer;
}

sf::SoundBuffer ResourceManager::generateGameMusicBuffer() {
    std::vector<sf::Int16> samples;
    unsigned int sampleRate = 22050;
    float duration = 4.0f;
    unsigned int sampleCount = static_cast<unsigned int>(duration * sampleRate);
    samples.reserve(sampleCount);

    float chords[2][4] = {
        { 220.0f, 261.63f, 329.63f, 392.00f }, // Am7
        { 146.83f, 174.61f, 220.00f, 261.63f }  // Dm7
    };

    for (unsigned int i = 0; i < sampleCount; ++i) {
        double t = static_cast<double>(i) / sampleRate;
        int chordIndex = static_cast<int>(t / 2.0) % 2;
        
        double beatTime = fmod(t, 0.5);
        double kickFreq = 120.0 * exp(-15.0 * beatTime);
        double kick = sin(2.0 * M_PI * kickFreq * beatTime) * exp(-6.0 * beatTime);

        int noteIndex = static_cast<int>(t / 0.125) % 4;
        double arpFreq = chords[chordIndex][noteIndex] * 2.0;
        double arpSin = sin(2.0 * M_PI * arpFreq * t);
        double arp = (arpSin > 0.0) ? 1.0 : -1.0;
        double env = exp(-12.0 * fmod(t, 0.125));

        double mixed = 0.4 * kick + 0.08 * arp * env;
        
        if (mixed > 1.0) mixed = 1.0;
        if (mixed < -1.0) mixed = -1.0;
        samples.push_back(static_cast<sf::Int16>(mixed * 32767));
    }

    sf::SoundBuffer buffer;
    buffer.loadFromSamples(&samples[0], samples.size(), 1, sampleRate);
    return buffer;
}

sf::SoundBuffer ResourceManager::generateEatSoundBuffer() {
    std::vector<sf::Int16> samples;
    unsigned int sampleRate = 22050;
    float duration = 0.12f;
    unsigned int sampleCount = static_cast<unsigned int>(duration * sampleRate);
    samples.reserve(sampleCount);

    for (unsigned int i = 0; i < sampleCount; ++i) {
        double t = static_cast<double>(i) / sampleRate;
        double freq = 300.0 + 600.0 * (t / duration);
        double val = sin(2.0 * M_PI * freq * t) * exp(-4.0 * t);
        samples.push_back(static_cast<sf::Int16>(val * 0.3 * 32767));
    }

    sf::SoundBuffer buffer;
    buffer.loadFromSamples(&samples[0], samples.size(), 1, sampleRate);
    return buffer;
}

sf::SoundBuffer ResourceManager::generateClickSoundBuffer() {
    std::vector<sf::Int16> samples;
    unsigned int sampleRate = 22050;
    float duration = 0.04f;
    unsigned int sampleCount = static_cast<unsigned int>(duration * sampleRate);
    samples.reserve(sampleCount);

    for (unsigned int i = 0; i < sampleCount; ++i) {
        double t = static_cast<double>(i) / sampleRate;
        double val = sin(2.0 * M_PI * 1500.0 * t) * exp(-50.0 * t);
        samples.push_back(static_cast<sf::Int16>(val * 0.25 * 32767));
    }

    sf::SoundBuffer buffer;
    buffer.loadFromSamples(&samples[0], samples.size(), 1, sampleRate);
    return buffer;
}

sf::SoundBuffer ResourceManager::generateGameOverSoundBuffer() {
    std::vector<sf::Int16> samples;
    unsigned int sampleRate = 22050;
    float duration = 0.8f;
    unsigned int sampleCount = static_cast<unsigned int>(duration * sampleRate);
    samples.reserve(sampleCount);

    for (unsigned int i = 0; i < sampleCount; ++i) {
        double t = static_cast<double>(i) / sampleRate;
        double freq = 350.0 - 260.0 * (t / duration);
        if (freq < 90.0) freq = 90.0;
        double val = sin(2.0 * M_PI * freq * t) * (1.0 - t / duration);
        samples.push_back(static_cast<sf::Int16>(val * 0.35 * 32767));
    }

    sf::SoundBuffer buffer;
    buffer.loadFromSamples(&samples[0], samples.size(), 1, sampleRate);
    return buffer;
}

sf::SoundBuffer ResourceManager::generateUnlockSoundBuffer() {
    std::vector<sf::Int16> samples;
    unsigned int sampleRate = 22050;
    float duration = 0.5f;
    unsigned int sampleCount = static_cast<unsigned int>(duration * sampleRate);
    samples.reserve(sampleCount);

    // Triad frequencies: C5, E5, G5, C6
    float notes[4] = { 523.25f, 659.25f, 783.99f, 1046.50f };

    for (unsigned int i = 0; i < sampleCount; ++i) {
        double t = static_cast<double>(i) / sampleRate;
        int noteIndex = static_cast<int>(t / 0.125f);
        if (noteIndex > 3) noteIndex = 3;
        double freq = notes[noteIndex];
        
        double val = sin(2.0 * M_PI * freq * t) * exp(-4.0 * fmod(t, 0.125f));
        samples.push_back(static_cast<sf::Int16>(val * 0.3f * 32767));
    }

    sf::SoundBuffer buffer;
    buffer.loadFromSamples(&samples[0], samples.size(), 1, sampleRate);
    return buffer;
}
