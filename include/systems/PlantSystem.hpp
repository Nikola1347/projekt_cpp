#pragma once
#include <SFML/Graphics.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <map>
#include <vector>
#include <string>
#include <iostream>

// PLANTDEFINITION - DEFINICJA ROŚLINY
struct PlantDefinition
{
    std::vector<sf::IntRect> stages; // klatki animacji wzrostu
    std::vector<float> growTimes; // czas do kolejnego etapu
};

// STAN POLA UPRAWNEGO
enum class FieldState
{
    Empty,
    NeedsWater,
    Growing,
    ReadyToHarvest
};

// POJEDYNCZE POLE UPRAWNE
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

// PLANTSYSTEM - SYSTEM ZARZĄDZAJĄCY POLAMI I ROŚLINAMI
struct PlantSystem
{
    PlantSystem();
 
    // WCZYTUJE POLA I ROŚLINY Z MAPY TILED
    void load(const tmx::Map& map);

    // AKTUALIZUJE WZROST ROŚLIN
    void update(float dt);

    // RYSUJE WSZYSTKIE ROŚLINY NA MAPIE
    void draw(sf::RenderWindow& window);
 
    // SADZENIE ROŚLINY NA POLU
    void plant(int id);

    // PODLEWANIE POLA
    void water(int id);

    // ZBIERANIE PLONÓW
    void harvest(int id);

    // ZWRACA STAN POLA
    FieldState getFieldState(int id);
 
    sf::Texture texture;
    int tileSize = 16;
 
    std::map<std::string, PlantDefinition> plantDefs;
    std::vector<Field> fields;

    // ZWRACA NAZWĘ ROŚLINY NA POLU
    std::string getPlantName(int fieldID);

    // ZWRACA CZAS DO KOLEJNEGO ETAPU WZROSTU
    float getTimeToNextStage(int fieldID);

    // MAGAZYN ZBIORÓW
    std::map<std::string, int> storage;
    const std::map<std::string, int>& getStorage() const { return storage; }
};