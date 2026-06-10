#pragma once
#include "core/State.hpp"
#include <SFML/Graphics.hpp>

// HELPSTATE - EKRAN POMOCY WYŚWIETLAJĄCY INFORMACJE O STEROWANIU
struct HelpState : State {
    HelpState();

    void handleInput(Game& game) override;
    void update(Game& game) override;
    void draw(Game& game) override;

    sf::Font font;
    sf::RectangleShape background;
    sf::Text title;
    sf::RectangleShape box;
    sf::Text text;
};