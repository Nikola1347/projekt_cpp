#pragma once
#include "State.hpp"
#include "../entities/Player.hpp"
#include <SFML/Graphics.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>

struct PlayState : State {
    PlayState();

    void handleInput(Game& game) override;
    void update(Game& game) override;
    void draw(Game& game) override;

    sf::Font font;
    sf::Text text;
    Player player;
    sf::View camera;
    sf::Clock dtClock;

    tmx::Map mapa;
    std::vector<sf::VertexArray> warstwy;
    sf::Vector2u mapSizePixels;
    sf::Texture tileset;
    sf::Texture tilesetFence;
};