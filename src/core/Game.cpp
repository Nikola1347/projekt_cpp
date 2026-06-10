#include "core/Game.hpp"
#include "states/MenuState.hpp"

// KONSTRUKTOR - TWORZY OKNO I USTAWIA POCZĄTKOWY STAN
Game::Game() {
    window.create(sf::VideoMode(800, 600), "Symulator farmy");
    pushState(std::make_unique<MenuState>());
}

// DODAWANIE NOWEGO STANU NA STOS
void Game::pushState(std::unique_ptr<State> state) {
    states.push_back(std::move(state)); 
}

// USUWANIE STANU ZE SZCZYTU STOSU
void Game::popState() {
    if(!states.empty())
        states.pop_back();
}

// POBIERANIE AKTUALNEGO STANU
State* Game::currentState() {
    if(states.empty()) return nullptr;
    return states.back().get();
}

// GŁÓWNA PĘTLA GRY
void Game::run() {
    while(window.isOpen()) {

        State* state = currentState();
        if(!state) continue;

        state->handleInput(*this);
        state = currentState();

        if(!state) continue;

        state->update(*this);

        window.clear();

        for (auto& s : states)
            s->draw(*this);

        window.display();
    }
}

// PRZEJŚCIE DO MENU GŁÓWNEGO
void Game::goToMenu() {
    states.clear();
    pushState(std::make_unique<MenuState>());
}

// ZWRACA STAN POD AKTUALNYM
State* Game::getStateBelowTop()
{
    if (states.size() < 2) return nullptr;
    return states[states.size() - 2].get();
}

// POBIERANIE PLAYSTATE ZE STOSU
PlayState* Game::getPlayState()
{
    for (auto& s : states)
        if (auto p = dynamic_cast<PlayState*>(s.get()))
            return p;
    return nullptr;
}

// WCZYTUJE STAN GRY Z ZAPISU
void Game::loadPlayStateFromSave()
{
    states.clear();
    auto play = std::make_unique<PlayState>();

    if (SaveSystem::load(*play))
    {
        pushState(std::move(play));
    }
    else
    {
        pushState(std::make_unique<MenuState>());
    }
}