#include "core/SaveSystem.hpp"
#include <fstream>
#include <iostream>

void SaveSystem::save(PlayState& state)
{
    json j;

    // złoto
    j["gold"] = state.gold;

    // pozycja gracza
    j["player"]["x"] = state.player.sprite.getPosition().x;
    j["player"]["y"] = state.player.sprite.getPosition().y;

    // magazyn roślin
    for (auto& [name, amount] : state.plantSystem.storage)
        j["storage"][name] = amount;

    // pola uprawne
    for (auto& f : state.plantSystem.fields)
    {
        json field;
        field["id"] = f.id;
        field["plantType"] = f.plantType;
        field["growthStage"] = f.growthStage;
        field["timer"] = f.timer;
        field["state"] = (int)f.state;

        j["fields"].push_back(field);
    }

    // zapis do pliku
    std::ofstream file("../data/save.json");
    file << j.dump(4);
}

// WCZYTUJE STAN GRY Z PLIKU save.json
bool SaveSystem::load(PlayState& state)
{
    std::ifstream file("../data/save.json");
    if (!file.is_open())
        return false;

    json j;
    file >> j;

    // złoto
    state.gold = j["gold"];

    // pozycja gracza
    float px = j["player"]["x"];
    float py = j["player"]["y"];
    state.player.pos = { px, py };
    state.player.sprite.setPosition(state.player.pos);

    // magazyn
    state.plantSystem.storage.clear();
    for (auto& [name, amount] : j["storage"].items())
        state.plantSystem.storage[name] = amount;

    // pola uprawne
    auto& fields = state.plantSystem.fields;
    auto& jfields = j["fields"];

    std::size_t count = std::min(fields.size(), jfields.size());

    for (std::size_t i = 0; i < count; ++i)
    {
        auto& f  = fields[i];
        auto& jf = jfields[i];

        f.id          = jf["id"];
        f.plantType   = jf["plantType"];
        f.growthStage = jf["growthStage"];
        f.timer       = jf["timer"];
        f.state       = (FieldState)(int)jf["state"];
    }

    return true;
}