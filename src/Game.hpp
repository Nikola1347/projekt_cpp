#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "State.hpp"

struct Game {
    Game();
    
    void pushState(std::unique_ptr<State> state);
    void popState();
    State* currentState();

    void run();
    void goToMenu();

    sf::RenderWindow window;
    std::vector<std::unique_ptr<State>> states;
};