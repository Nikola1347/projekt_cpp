#pragma once

struct Game;

// STATE - INTERFEJS DLA WSZYSTKICH STANÓW GRY
struct State {
    virtual ~State() = default;

    // OBSŁUGA WEJŚCIA UŻYTKOWNIKA
    virtual void handleInput(Game& game) = 0;

    // AKTUALIZACJA LOGIKI STANU
    virtual void update(Game& game) = 0;

    // RYSOWANIE ELEMENTÓW STANU
    virtual void draw(Game& game) = 0;
};