#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "State.hpp"

class Game {
public:
    Game();
    void pushState(std::unique_ptr<State> state);
    void popState();
    State* currentState();

    void run();

    sf::RenderWindow window;

private:
    std::vector<std::unique_ptr<State>> states;
};