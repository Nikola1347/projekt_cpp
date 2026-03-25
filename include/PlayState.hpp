#pragma once
#include "State.hpp"
#include <SFML/Graphics.hpp>

class PlayState : public State {
public:
    PlayState();

    void handleInput(Game& game) override;
    void update(Game& game) override;
    void draw(Game& game) override;

private:
    sf::Font font;
    sf::Text text;
};