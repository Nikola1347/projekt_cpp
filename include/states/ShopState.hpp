#pragma once
#include "core/State.hpp"
#include "systems/PlantSystem.hpp"
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <cstring>

// SHOPSTATE - STAN SKLEPU DO KUPNA I SPRZEDAŻY ROŚLIN
struct ShopState : State
{
    ShopState(int& goldRef, PlantSystem& plantSystemRef);

    void handleInput(Game& game) override;
    void update(Game& game) override;
    void draw(Game& game) override;

    // referencje do danych gry
    int& gold;
    PlantSystem& plantSystem;

    // UI
    sf::Font font;
    sf::RectangleShape box;
    sf::Text title;

    std::vector<std::string> options;
    std::vector<sf::RectangleShape> boxes;
    std::vector<sf::Text> texts;

    int selected = 0;

    // ceny
    std::map<std::string, int> prices;
    std::map<std::string, int> sellPrices;
};