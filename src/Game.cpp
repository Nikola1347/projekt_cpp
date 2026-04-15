#include "Game.hpp"
#include "MenuState.hpp"

Game::Game() {
    window.create(sf::VideoMode(800, 600), "Symulator farmy");
    pushState(std::make_unique<MenuState>());
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

        State* state = currentState();
        if(!state) continue;

        state->handleInput(*this);
        state = currentState();

        if(!state) continue;

        state->update(*this);

        window.clear();
        state->draw(*this);
        window.display();
    }
}

void Game::goToMenu() {
    states.clear();
    pushState(std::make_unique<MenuState>());
}