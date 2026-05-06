#pragma once
#include <SFML/Graphics.hpp>

struct Animation {
    sf:: Texture texture;
    int frameCount;
    int frameWidth;
    int frameHeight;
    float frameTime;
};

struct Player {
    sf::Sprite sprite;
    sf::Vector2f pos;

    Animation walkAnim[3];
    Animation idleAnim[3];

    float animTimer = 0.f;
    int currentFrame = 0;
    int currentDir = 0;
    bool walking = false;

    void loadAnimations();
    void update(float dt);

    sf::FloatRect getHitbox() const;
};