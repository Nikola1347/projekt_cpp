#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "State.hpp"
#include "core/SaveSystem.hpp"

// GAME - GŁÓWNY MENEDŻER OKNA I STANÓW GRY
struct Game {
    Game();
    
    // ZARZĄDZANIE STANAMI
    void pushState(std::unique_ptr<State> state);
    void popState();
    State* currentState();

    // GłÓWNA PĘTLA GRY
    void run();

    // PRZEJŚCIE DO MENU
    void goToMenu();

    sf::RenderWindow window;
    std::vector<std::unique_ptr<State>> states;

    // POMOCNICZE DOSTĘPY DO STANÓW
    State* getStateBelowTop();
    PlayState* getPlayState();
    void loadPlayStateFromSave();

    template<typename T>
    bool isTop() const {
        if (states.empty()) return false;
        return dynamic_cast<T*>(states.back().get()) != nullptr;
    }
};