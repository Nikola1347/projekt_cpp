#pragma once
#include "State.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

struct MenuState : State {
    MenuState();

    void handleInput(Game& game) override;
    void update(Game& game) override;
    void draw(Game& game) override;

    sf::Font font;
    sf::RectangleShape background;
    sf::Text title;
    std::vector<sf::Text> texts;
    std::vector<sf::RectangleShape> boxes;
    int selected = 0;
};