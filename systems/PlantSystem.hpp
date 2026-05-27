#pragma once
#include <SFML/Graphics.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <map>
#include <vector>
#include <string>
 
struct PlantDefinition
{
    std::vector<sf::IntRect> stages;
    std::vector<float> growTimes;
};
 
enum class FieldState
{
    Empty,
    NeedsWater,
    Growing,
    ReadyToHarvest
};
 
struct Field
{
    int id;
    std::string plantType; //ustalony z mapy
    sf::FloatRect area;
 
    FieldState state = FieldState::Empty;
    int growthStage = 0;
    float timer = 0.f;
 
    std::vector<sf::Vector2f> plantPositions;
};
 
struct PlantSystem
{
    PlantSystem();
 
    void load(const tmx::Map& map);
    void update(float dt);
    void draw(sf::RenderWindow& window);
 
    void plant(int id);
    void water(int id);
    void harvest(int id);
    FieldState getFieldState(int id);
 
    sf::Texture texture;
    int tileSize = 16;
 
    std::map<std::string, PlantDefinition> plantDefs;
    std::vector<Field> fields;
    std::string getPlantName(int fieldID);
    float getTimeToNextStage(int fieldID);
};