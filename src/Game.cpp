#include "Game.hpp"

Game::Game()
: window(sf::VideoMode(800, 600), "gra x")
{
}

void Game::pushState(std::unique_ptr<State> state) {
    states.push_back(std::move(state));
}

void Game::popState() {
    if(!states.empty())
        states.pop_back();
}

State* Game::currentState() {
    if(states.empty()) return nullptr;
    return states.back().get();
}

void Game::run() {
    while(window.isOpen()) {

        if(currentState()) {
            currentState()->handleInput(*this);
            currentState()->update(*this);
        }

        window.clear(sf::Color::Black);

        if(currentState())
            currentState()->draw(*this);

        window.display();
    }
}