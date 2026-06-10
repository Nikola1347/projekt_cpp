#pragma once
#include "states/PlayState.hpp"
#include "core/json.hpp"

using json = nlohmann::json;

// SAVESYSTEM - ZAPIS I ODCZYT STANU GRY DO PLIKU JSON
namespace SaveSystem
{
    // ZAPISUJE STAN GRY DO save.json
    void save(PlayState& state);

    // WCZYTUJE STAN GRY Z save.json
    bool load(PlayState& state);
}