#pragma once
#include <SFML/Graphics.hpp>

struct Player {
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f pos;

    Player();
};