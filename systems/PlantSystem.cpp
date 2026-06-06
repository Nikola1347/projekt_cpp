#include "PlantSystem.hpp"
#include <cstdlib>
#include <iostream>
 
PlantSystem::PlantSystem()
{
    tileSize = 16;
 
    // DEFINICJE ROŚLIN
    // stages = klatki tekstury
    // growTimes = czas [s] do następnego etapu
 
    // TRUSKAWKA (rząd 1)
    plantDefs["truskawka"] = {
        {
            {1*16, 1*16, 16, 16},
            {2*16, 1*16, 16, 16},
            {3*16, 1*16, 16, 16},
            {5*16, 1*16, 16, 16}
        },
        { 3.f, 6.f, 12.f } //zmień na 30.f, 60.f, 120.f
    };
 
    // POR (rząd 3)
    plantDefs["por"] = {
        {
            {2*16, 3*16, 16, 16},
            {3*16, 3*16, 16, 16},
            {4*16, 3*16, 16, 16},
            {5*16, 3*16, 16, 16}
        },
        { 4.f, 9.f, 18.f } //zmień na 45.f, 90.f, 180.f
    };
 
    // ZIEMNIAK (rząd 5)
    plantDefs["ziemniak"] = {
        {
            {2*16, 5*16, 16, 16},
            {3*16, 5*16, 16, 16},
            {4*16, 5*16, 16, 16},
            {6*16, 5*16, 16, 16}
        },
        { 4.f, 8.f, 16.f } //zmień na 40.f, 80.f, 160.f
    };
 
    // CEBULA (rząd 7)
    plantDefs["cebula"] = {
        {
            {2*16, 7*16, 16, 16},
            {3*16, 7*16, 16, 16},
            {4*16, 7*16, 16, 16},
            {5*16, 7*16, 16, 16}
        },
        { 3.f, 7.f, 14.f } //zmień na 35.f, 70.f, 140.f
    };
}

// wycztywanie
void PlantSystem::load(const tmx::Map& map)
{
    if (!texture.loadFromFile("assets/maps/Spring Crops.png"))
        std::cout << "Blad ladowania Spring Crops.png" << std::endl;

    for (const auto& layer : map.getLayers())
    {
        if (layer->getName() == "rosliny" && layer->getType() == tmx::Layer::Type::Object)
        {
            const auto& objLayer = layer->getLayerAs<tmx::ObjectGroup>();

            for (const auto& obj : objLayer.getObjects())
            {
                Field f;

                for (const auto& p : obj.getProperties())
                {
                    if (p.getName() == "fieldID")
                        f.id = p.getIntValue();

                    if (p.getName() == "plantType")
                        f.plantType = p.getStringValue();
                }

                auto aabb = obj.getAABB();

                // pozycja i rozmiar obiektu z Tiled
                f.area = sf::FloatRect(
                    aabb.left,
                    aabb.top,
                    aabb.width,
                    aabb.height
                );

                // zabezpieczenie przed zerowym rozmiarem
                if (f.area.width <= 0.f || f.area.height <= 0.f)
                {
                    std::cout << "pole id=" << f.id << " ma zerowy rozmiar\n";
                    continue;
                }

                // pozycja roślinek wewnątrz pola
                int cols = 4;
                int rows = 5;

                float cellW = f.area.width  / cols;
                float cellH = f.area.height / rows;

                for (int r=0; r<rows; r++)
                {
                    for (int c=0; c<cols; c++)
                    {
                        float x = f.area.left + c * cellW + cellW * 0.5f - 8.f;
                        float y = f.area.top  + r * cellH + cellH * 0.5f - 8.f;

                        f.plantPositions.push_back({x, y});
                    }
                }
 
                f.state = FieldState::Empty;
                f.growthStage = 0;
                f.timer = 0.f;
                fields.push_back(f);
            }
        }
    }
}

// update
void PlantSystem::update(float dt)
{
    for (auto& f : fields)
    {
        if (f.state != FieldState::Growing)
            continue;
 
        if (plantDefs.find(f.plantType) == plantDefs.end())
            continue;
 
        auto& def = plantDefs[f.plantType];
        int maxStage = (int)def.stages.size() - 1;
 
        if (f.growthStage >= maxStage)
            continue;
 
        f.timer += dt;
 
        float needed = def.growTimes[f.growthStage];
        if (f.timer >= needed)
        {
            f.timer = 0.f;
            f.growthStage++;
 
            if (f.growthStage >= maxStage)
                f.state = FieldState::ReadyToHarvest;
            else
                f.state = FieldState::NeedsWater; // trzeba podlać przed kolejnym etapem
        }
    }
}

// SADZENIE
void PlantSystem::plant(int id)
{
    for (auto& f : fields)
    {
        if (f.id == id && f.state == FieldState::Empty)
        {
            f.growthStage = 0;
            f.timer = 0.f;
            f.state = FieldState::NeedsWater; // zaraz po zasadzeniu trzeba podlać
        }
    }
}

// PODLEWANIE
void PlantSystem::water(int id)
{
    for (auto& f : fields)
    {
        if (f.id == id && f.state == FieldState::NeedsWater)
        {
            f.state = FieldState::Growing;
            f.timer = 0.f;
        }
    }
}

// ZBIERANIE
void PlantSystem::harvest(int id)
{
    for (auto& f : fields)
    {
        if (f.id == id && f.state == FieldState::ReadyToHarvest)
        {
            // dodawanie do magazynu
            storage[f.plantType] += 10;
            std::cout << "Dodano 10x " << f.plantType << " do magazynu\n";
            
            f.growthStage = 0;
            f.timer = 0.f;
            f.state = FieldState::Empty;
        }
    }
}

// id pola
FieldState PlantSystem::getFieldState(int id)
{
    for (auto& f : fields)
        if (f.id == id)
            return f.state;

    return FieldState::Empty;
}

// nazwy rośliny
std::string PlantSystem::getPlantName(int fieldID)
{
    for (auto& f : fields)
    {
        if (f.id == fieldID)
            return f.plantType;
    }
    return "";
}

// RYSOWANIE
void PlantSystem::draw(sf::RenderWindow& window)
{
    for (auto& f : fields)
    {
        if (f.state == FieldState::Empty)
            continue;

        if (plantDefs.find(f.plantType) == plantDefs.end())
            continue;

        auto& def = plantDefs[f.plantType];
        int stage = std::min(f.growthStage, (int)def.stages.size() - 1);

        for (auto& pos : f.plantPositions)
        {
            sf::Sprite spr;
            spr.setTexture(texture);
            spr.setTextureRect(def.stages[stage]);
            spr.setPosition(pos);
            window.draw(spr);
        }
    }
}

// TIMER
float PlantSystem::getTimeToNextStage(int fieldID)
{
    for (auto& f : fields)
    {
        if (f.id == fieldID)
        {
            // jeśli roślina nie rośnie = nie ma licznika
            if (f.state != FieldState::Growing)
                return 0.f;

            if (plantDefs.find(f.plantType) == plantDefs.end())
                return 0.f;

            auto& def = plantDefs[f.plantType];

            // jeśli ostatni etap = brak licznika
            if (f.growthStage >= (int)def.growTimes.size())
                return 0.f;

            float needed = def.growTimes[f.growthStage];
            float remaining = needed - f.timer;

            return std::max(0.f, remaining);
        }
    }
    return 0.f;
}