#pragma once

struct Game;

struct State {
    virtual ~State() = default;
    virtual void handleInput(Game& game) = 0;
    virtual void update(Game& game) = 0;
    virtual void draw(Game& game) = 0;
};