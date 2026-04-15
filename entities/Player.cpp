#include "player.hpp"

Player::Player() {
    texture.loadFromFile("assets/textures/farmer_test.png");
    sprite.setTexture(texture);

    sf::FloatRect b = sprite.getLocalBounds();
    sprite.setOrigin(b.width / 2, b.height / 2);

    pos = { 400.f, 300.f };
}