#include "core/Game.hpp"
#include "states/MenuState.hpp"

// GŁÓWNA FUNKCJA PROGRAMU - URUCHAMIA GRĘ
int main() {
    Game game; // tworzy obiekt gry i okno

    // USTAWIENIE POCZĄTKOWEGO STANU
    game.pushState(std::make_unique<MenuState>());

    // URUCHOMIENIE GŁÓWNEJ PĘTLI GRY
    game.run();

    return 0;
}