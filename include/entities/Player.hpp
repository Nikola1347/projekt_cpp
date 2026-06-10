#pragma once
#include <SFML/Graphics.hpp>

// ANIMATION - POJEDYNCZA ANIMACJA
struct Animation {
    sf:: Texture texture;
    int frameCount;
    int frameWidth;
    int frameHeight;
    float frameTime;
};

// PLAYER - OBIEKT GRACZA Z ANIMACJAMI I POZYCJĄ
struct Player {
    sf::Sprite sprite;
    sf::Vector2f pos;

    Animation walkAnim[3]; // animacje chodzenia (dół, góra, bok)
    Animation idleAnim[3]; // animacje stania

    float animTimer = 0.f;
    int currentFrame = 0;
    int currentDir = 0;
    bool walking = false;

    // WCZYTUJE ANIMACJE GRACZA
    void loadAnimations();

    // AKTUALIZUJE ANIMACJĘ I POZYCJĘ SPRITE'A
    void update(float dt);

    // ZWRACA HITBOX GRACZA DO KOLIZJI
    sf::FloatRect getHitbox() const;
};