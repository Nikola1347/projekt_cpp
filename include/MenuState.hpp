#pragma once
#include "State.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class MenuState : public State {
public:
    MenuState();

    void handleInput(Game& game) override;
    void update(Game& game) override;
    void draw(Game& game) override;

private:
    sf::Font font;
    std::vector<sf::Text> options;
    int selected = 0;
};