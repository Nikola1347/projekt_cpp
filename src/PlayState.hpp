#pragma once
#include "State.hpp"
#include "../entities/Player.hpp"
#include "../systems/timesystem.hpp"
#include <SFML/Graphics.hpp>

struct PlayState : State {
    PlayState();

    void handleInput(Game& game) override;
    void update(Game& game) override;
    void draw(Game& game) override;

    sf::Font font;
    sf::Text text;
    sf::Texture mapTexture;
    sf::Sprite mapSprite;
    Player player;
    sf::View camera;
    TimeSystem timeSystem;
    sf::Clock dtClock;
};