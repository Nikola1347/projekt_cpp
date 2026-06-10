#include "systems/PlantSystem.hpp"
#include <cstdlib>

// KONSTRUKTOR - TWORZY DEFINICJE ROŚLIN
PlantSystem::PlantSystem()
{
    tileSize = 16;
 
    // DEFINICJE ROŚLIN
    // stages = klatki tekstury
    // growTimes = czas [s] do następnego etapu
 
    // TRUSKAWKA
    plantDefs["truskawka"] = {
        {
            {1*16, 1*16, 16, 16},
            {2*16, 1*16, 16, 16},
            {3*16, 1*16, 16, 16},
            {5*16, 1*16, 16, 16}
        },
        { 3.f, 6.f, 12.f }
    };
 
    // POR
    plantDefs["por"] = {
        {
            {2*16, 3*16, 16, 16},
            {3*16, 3*16, 16, 16},
            {4*16, 3*16, 16, 16},
            {5*16, 3*16, 16, 16}
        },
        { 4.f, 9.f, 18.f }
    };
 
    // ZIEMNIAK
    plantDefs["ziemniak"] = {
        {
            {2*16, 5*16, 16, 16},
            {3*16, 5*16, 16, 16},
            {4*16, 5*16, 16, 16},
            {6*16, 5*16, 16, 16}
        },
        { 4.f, 8.f, 16.f }
    };
 
    // CEBULA
    plantDefs["cebula"] = {
        {
            {2*16, 7*16, 16, 16},
            {3*16, 7*16, 16, 16},
            {4*16, 7*16, 16, 16},
            {5*16, 7*16, 16, 16}
        },
        { 3.f, 7.f, 14.f }
    };
}

// WCZYTUJE POLA UPRAWNE Z MAPY TILED
void PlantSystem::load(const tmx::Map& map)
{
    if (!texture.loadFromFile("assets/maps/Spring Crops.png"))
        std::cout << "Blad ladowania Spring Crops.png" << std::endl;

    fields.clear();
    storage.clear();
    
    for (const auto& layer : map.getLayers())
    {
        if (layer->getName() == "rosliny" && layer->getType() == tmx::Layer::Type::Object)
        {
            const auto& objLayer = layer->getLayerAs<tmx::ObjectGroup>();

            for (const auto& obj : objLayer.getObjects())
            {
                Field f;

                // odczyt właściwości pola
                for (const auto& p : obj.getProperties())
                {
                    if (p.getName() == "fieldID")
                        f.id = p.getIntValue();

                    if (p.getName() == "plantType")
                        f.plantType = p.getStringValue();
                }

                // pozycja i rozmiar obiektu
                auto aabb = obj.getAABB();
                f.area = sf::FloatRect(aabb.left, aabb.top, aabb.width, aabb.height);

                // generowanie pozycji roślin w polu
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

// AKTUALIZUJE WZROST WSZYSTKICH ROŚLIN
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
                f.state = FieldState::NeedsWater;
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
            storage[f.plantType] += 1;

            f.growthStage = 0;
            f.timer = 0.f;
            f.state = FieldState::Empty;
        }
    }
}

// ZWRACA STAN POLA
FieldState PlantSystem::getFieldState(int id)
{
    for (auto& f : fields)
        if (f.id == id)
            return f.state;

    return FieldState::Empty;
}

// ZWRACA NAZWĘ ROŚLINY NA POLU
std::string PlantSystem::getPlantName(int fieldID)
{
    for (auto& f : fields)
    {
        if (f.id == fieldID)
            return f.plantType;
    }
    return "";
}

// RYSOWANIE WSZYSTKICH ROŚLIN
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

// ZWRACA CZAS DO KOLEJNEGO ETAPU WZROSTU
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