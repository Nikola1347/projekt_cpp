#pragma once
#include "core/State.hpp"
#include "entities/Player.hpp"
#include "systems/PlantSystem.hpp"
#include "ShopState.hpp"
#include <SFML/Graphics.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>

// PLAYSTATE - GŁÓWNY STAN ROZGRYWKI
struct PlayState : State {
    PlayState();

    void handleInput(Game& game) override;
    void update(Game& game) override;
    void draw(Game& game) override;

    // resetuje zegar delta-time
    void resetClock() { dtClock.restart(); }

    sf::Font font;
    sf::Text text;
    Player player;
    sf::View camera;
    sf::Clock dtClock;

    // mapa
    tmx::Map mapa;
    sf::Vector2u mapSizePixels;
    std::vector<sf::Texture> tilesetTextures;
    std::vector<unsigned> firstGIDs;

    // kolizje
    std::vector<sf::FloatRect> collisionRects;
    bool checkCollision(const sf::FloatRect& nextPos);
    std::unordered_map<int, sf::FloatRect> tileCollision;
    void drawTileLayer(const std::unique_ptr<tmx::Layer>& layer, sf::RenderWindow& window);

    // interakcje
    std::vector<sf::FloatRect> interactRects;

    // tabliczki
    struct Sign {
        int fieldID;
        sf::FloatRect area;
    };
    std::vector<Sign> signs;

    std::string getPlantName(int fieldID);

    // system roślinek
    PlantSystem plantSystem;
    bool paused = false;

    // sklep
    struct ShopArea {
        sf::FloatRect area;
    };
    std::vector<ShopArea> shops;
    
    int gold = 2;

    //licznik złota
    sf::Text goldText;
    sf::RectangleShape goldBox;
};