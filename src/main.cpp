#include "Game.hpp"
#include "MenuState.hpp"

int main() {
    Game game;
    game.pushState(std::make_unique<MenuState>());
    game.run();
}