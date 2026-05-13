#pragma once
#include "State.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

struct MiniMenuState : State
{
    MiniMenuState();
    
    void handleInput(Game& game) override;
    void update(Game& game) override;
    void draw(Game& game) override;

    sf::RectangleShape box;
    sf::Font font;
    std::vector<sf::RectangleShape> boxes;
    std::vector<sf::Text> texts;

    int selected = 0;
};