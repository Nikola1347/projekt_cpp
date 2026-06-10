#pragma once
#include "core/State.hpp"
#include <SFML/Graphics.hpp>

// STARTSTATE - EKRAN WPROWADZAJĄCY Z INSTRUKCJĄ GRY
struct StartState : State {
    StartState();

    void handleInput(Game& game) override;
    void update(Game& game) override;
    void draw(Game& game) override;

    sf::Font font;
    sf::RectangleShape background;
    sf::Text title;
    sf::RectangleShape box;
    sf::Text text;
    sf::Text continueText;
};