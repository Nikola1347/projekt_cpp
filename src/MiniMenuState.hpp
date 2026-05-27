#pragma once
#include "State.hpp"
#include "../systems/PlantSystem.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>
#include <string>
 
struct MiniMenuState : State
{
    MiniMenuState(
        int id,
        FieldState fieldState,
        const std::string& plantName,
        std::function<void(int)> onPlant,
        std::function<void(int)> onWater,
        std::function<void(int)> onHarvest
    );
 
    void handleInput(Game& game) override;
    void update(Game& game) override;
    void draw(Game& game) override;
 
    sf::RectangleShape box;
    sf::Font font;
    std::vector<sf::RectangleShape> boxes;
    std::vector<sf::Text> texts;
    std::vector<std::string> options;
    sf::Text title;
 
    int selected = 0;
    int fieldID;
    FieldState fieldState;
    std::string plantName;
    std::function<void(int)> onPlant;
    std::function<void(int)> onWater;
    std::function<void(int)> onHarvest;
};