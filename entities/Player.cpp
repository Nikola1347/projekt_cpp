#include "Player.hpp"

void Player::loadAnimations()
{
    // chodzenie
    walkAnim[0].texture.loadFromFile("assets/textures/Walk.png");
    walkAnim[0].frameCount = 6;
    walkAnim[0].frameWidth = 32;
    walkAnim[0].frameHeight = 32;
    walkAnim[0].frameTime = 0.2f;

    walkAnim[1] = walkAnim[0]; // góra
    walkAnim[2] = walkAnim[0]; // dół

    // stanie
    idleAnim[0].texture.loadFromFile("assets/textures/Idle.png");
    idleAnim[0].frameCount = 4;
    idleAnim[0].frameWidth = 32;
    idleAnim[0].frameHeight = 32;
    idleAnim[0].frameTime = 0.2f;

    idleAnim[1] = idleAnim[0];
    idleAnim[2] = idleAnim[0];

    sprite.setTexture(idleAnim[0].texture);
    sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
    sprite.setOrigin(16, 16);
}

void Player::update(float dt)
{
    animTimer += dt;

    Animation& anim = walking ? walkAnim[currentDir] : idleAnim[currentDir];

    if (animTimer >= anim.frameTime)
    {
        animTimer = 0.f;
        currentFrame = (currentFrame + 1) % anim.frameCount;
    }

    int frameX = currentFrame * anim.frameWidth;
    int frameY = currentDir * anim.frameHeight;

    sprite.setTexture(anim.texture);
    sprite.setTextureRect(sf::IntRect(frameX, frameY, anim.frameWidth, anim.frameHeight));
    sprite.setPosition(pos);
}