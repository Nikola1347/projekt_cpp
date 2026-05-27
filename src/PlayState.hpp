#pragma once
#include "State.hpp"
#include "../entities/Player.hpp"
#include "../systems/PlantSystem.hpp"
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

    //mapa
    tmx::Map mapa;
    sf::Vector2u mapSizePixels;
    std::vector<sf::Texture> tilesetTextures;
    std::vector<unsigned> firstGIDs;

    //kolizje
    std::vector<sf::FloatRect> collisionRects;
    bool checkCollision(const sf::FloatRect& nextPos);
    std::unordered_map<int, sf::FloatRect> tileCollision;
    void drawTileLayer(const std::unique_ptr<tmx::Layer>& layer, sf::RenderWindow& window);

    //interakcje
    std::vector<sf::FloatRect> interactRects;

    //tabliczki
    struct Sign {
        int fieldID;
        sf::FloatRect area;
    };

    std::vector<Sign> signs;
    std::string getPlantName(int fieldID);

    //system roślinek
    PlantSystem plantSystem;
};