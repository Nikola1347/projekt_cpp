#include "states/PlayState.hpp"
#include "core/Game.hpp"
#include "states/PauseState.hpp"
#include "states/MiniMenuState.hpp"
#include "states/MenuState.hpp"
#include "states/PauseState.hpp"
#include "systems/PlantSystem.hpp"
#include <iostream>
#include <cmath>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>

// KONSTRUKTOR - WCZYTUJE MAPĘ, GRACZA, SYSTEM ROŚLIN I INTERFEJS
PlayState::PlayState()
{
    // wczytywanie mapy
    if (!mapa.load("../assets/maps/mapa.tmx"))
        std::cout << "Blad: mapa.tmx" << std::endl;

    // ładowanie tilesetów
    const auto& tilesets = mapa.getTilesets();
    for (const auto& ts : tilesets)
    {
        std::cout << "Tileset path: " << ts.getImagePath() << std::endl;
    }

    if (!font.loadFromFile("../assets/fonts/arial.ttf"))
    {
        std::cout << "blad fontu w PlayState\n";
    }

    for (const auto& ts : tilesets)
    {
        sf::Texture tex;
        std::string path = ts.getImagePath();

        if (!tex.loadFromFile(path))
            std::cout << "Blad ladowania tilesetu: " << path << std::endl;

        tilesetTextures.push_back(std::move(tex));
        firstGIDs.push_back(ts.getFirstGID());
    }

    // wczytywanie kolizji z tilesetów
    for (const auto& ts : mapa.getTilesets())
    {
        const auto& tiles = ts.getTiles();
        for (const auto& tile : tiles)
        {
            const auto& objects = tile.objectGroup.getObjects();
            for (const auto& obj : objects)
            {
                if (obj.getShape() == tmx::Object::Shape::Rectangle)
                {
                    float ox = obj.getPosition().x;
                    float oy = obj.getPosition().y;
                    float w = obj.getAABB().width;
                    float h = obj.getAABB().height;

                    tileCollision[ts.getFirstGID() + tile.ID] = sf::FloatRect(ox, oy, w, h);
                }
            }
        }
    }

    // ładowanie playera
    player.loadAnimations();
    player.pos = { 670.f, 250.f };
    player.sprite.setPosition(player.pos);

    // obliczanie rozmiaru mapy
    auto tile = mapa.getTileSize();
    auto count = mapa.getTileCount();
    mapSizePixels.x = tile.x * count.x;
    mapSizePixels.y = tile.y * count.y;

    // kamera
    camera.setSize(400, 300);
    camera.setCenter(400, 300);

    // system roślin
    plantSystem = PlantSystem();
    plantSystem.load(mapa);

    // wczytywanie obiektów z mapy
    for (const auto& layer : mapa.getLayers())
    {
         // wczytywanie tabliczek z warstwy tab_obj
        if (layer->getName() == "tab_obj" && layer->getType() == tmx::Layer::Type::Object)
        {
            const auto& objLayer = layer->getLayerAs<tmx::ObjectGroup>();

            for (const auto& obj : objLayer.getObjects())
            {
                Sign s;

                // fieldID
                for (const auto& p : obj.getProperties())
                {
                    if (p.getName() == "fieldID")
                    {
                        s.fieldID = p.getIntValue();
                    }
                }

                // obszar interakcji
                auto aabb = obj.getAABB();
                s.area = sf::FloatRect(aabb.left, aabb.top, aabb.width, aabb.height);
                signs.push_back(s);
            }
        }

        // wczytywanie obszaru sklepu z tab_obj
        if (layer->getName() == "sklep_obj" && layer->getType() == tmx::Layer::Type::Object)
        {
            const auto& objLayer = layer->getLayerAs<tmx::ObjectGroup>();

            for (const auto& obj : objLayer.getObjects())
            {
                ShopArea s;
                auto aabb = obj.getAABB();
                s.area = sf::FloatRect(aabb.left, aabb.top, aabb.width, aabb.height);
                shops.push_back(s);
            }
        }
    }

    // INTERFEJS ZŁOTA
    // ramka
    goldBox.setSize({140.f, 40.f});
    goldBox.setFillColor(sf::Color(0, 80, 0, 200));
    goldBox.setOutlineColor(sf::Color::Black);
    goldBox.setOutlineThickness(2);
    goldBox.setOrigin(goldBox.getSize().x / 2.f, goldBox.getSize().y / 2.f);
    goldBox.setPosition(80.f, 30.f);

    // tekst
    goldText.setFont(font);
    goldText.setCharacterSize(20);
    goldText.setFillColor(sf::Color::White);
    goldText.setOutlineColor(sf::Color::Black);
    goldText.setOutlineThickness(2);
    goldText.setString(std::to_string(gold));

    sf::FloatRect gb = goldText.getLocalBounds();
    goldText.setOrigin(gb.left + gb.width * 0.5f, gb.top + gb.height * 0.5f);
    goldText.setPosition(goldBox.getPosition());
    goldText.move(-35.f, 0.f); // korekta wizualna

}

// OBSŁUGA WEJŚCIA UŻYTKOWNIKA
void PlayState::handleInput(Game& game) {
    sf::Event event;
    while (game.window.pollEvent(event)) {

        if (event.type == sf::Event::Closed)
            game.window.close();

        if (event.type == sf::Event::KeyPressed) {
            // pauza
            if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::M) {
                paused = true;
                auto pause = std::make_unique<PauseState>();
                game.pushState(std::move(pause));
                return;
            }
        }    
    }
}

// SPRAWDZANIE KOLIZJI GRACZA Z OBIEKTAMI
bool PlayState::checkCollision(const sf::FloatRect& nextPos)
{
    for (const auto& rect : collisionRects)
    {
        if (rect.intersects(nextPos))
            return true;
    }
    return false;
}

// AKTUALIZACJA LOGIKI ROZGRYWKI
void PlayState::update(Game& game) {

    // ZATRZYMANIE GRY PODCZAS PAUZY LUB MENU
    if (game.isTop<PauseState>() || game.isTop<MenuState>()) {
        dtClock.restart();
        return;
    }

    // odnowienie listy kolizji
    collisionRects.clear();

    const auto& layers = mapa.getLayers();
    auto tileSize = mapa.getTileSize();
    auto mapSize = mapa.getTileCount();

    // PRZETWARZANIE WARSTW MAPY I KOLIZJI
    for (const auto& layer : layers)
    {
        bool layerHasCollision = false;

        // sprawdzamy, czy warstwa ma właściwość collision
        for (const auto& p : layer->getProperties())
        {
            if (p.getName() == "collision" && p.getBoolValue())
                layerHasCollision = true;
        }

        const tmx::TileLayer* tileLayer = dynamic_cast<const tmx::TileLayer*>(layer.get());
        if (!tileLayer)
            continue;

        const auto& tiles = tileLayer->getTiles();

        // iteracja po kafelkach
        for (unsigned y=0; y<mapSize.y; y++)
        {
            for (unsigned x=0; x<mapSize.x; x++)
            {
                std::size_t index = x + y * mapSize.x;
                const auto& tile = tiles[index];

                int gid = tile.ID;
                if (gid == 0)
                    continue;

                int tsIndex = -1;
                for (int i=0; i<firstGIDs.size(); i++)
                {
                    if (gid >= firstGIDs[i])
                        tsIndex = i;
                    else
                        break;
                }

                if (tsIndex == -1)
                    continue;

                // sprawdzanie kolizji kafelka
                if (tileCollision.find(gid) != tileCollision.end())
                {
                    const auto& col = tileCollision[gid];

                    sf::FloatRect worldCol(
                        x * tileSize.x + col.left,
                        y * tileSize.y + col.top,
                        col.width,
                        col.height
                    );

                    if (layerHasCollision)
                        collisionRects.push_back(worldCol);
                }
            }
        }
    }

    // RUCH GRACZA I DELTA TIME
    float dt = dtClock.restart().asSeconds();
    float speed = 100.f;
    player.walking = false;

    // GÓRA
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        float move = speed * dt;
        sf::FloatRect next = player.getHitbox();
        next.top -= move;

        if (!checkCollision(next))
            player.pos.y -= move;

        player.currentDir = 1;
        player.walking = true;
        player.sprite.setScale(1.f, 1.f);
    }

    // DÓŁ
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        float move = speed * dt;
        sf::FloatRect next = player.getHitbox();
        next.top += move;
        
        if (!checkCollision(next))
            player.pos.y += move;

        player.currentDir = 0;
        player.walking = true;
        player.sprite.setScale(1.f, 1.f);
    }

    // LEWO
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        float move = speed * dt;
        sf::FloatRect next = player.getHitbox();
        next.left -= move;
        
        if (!checkCollision(next))
            player.pos.x -= move;
        
        player.currentDir = 2;
        player.walking = true;
        player.sprite.setScale(-1.f, 1.f);
    }

    // PRAWO
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        float move = speed * dt;
        sf::FloatRect next = player.getHitbox();
        next.left += move;

        if (!checkCollision(next))
            player.pos.x += move;

        player.currentDir = 2;
        player.walking = true;
        player.sprite.setScale(1.f, 1.f);
    }

    // aktualizacja animacji gracza
    player.update(dt);

    // aktualizacja roślin (tylko gdy gra nie jest zapauzowana)
    if (!game.isTop<PauseState>() && !game.isTop<MenuState>())
    {
        plantSystem.update(dt);
    }

    // INTERAKCJA ZE SKLEPEM
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
    {
        for (const auto& shop : shops)
        {
            if (shop.area.intersects(player.getHitbox()))
            {
                game.pushState(std::make_unique<ShopState>(gold, plantSystem));
                return;
            }
        }
    }

    // INTERAKCJA Z POLAMI (MINI MENU)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
    {
        for (const auto& s : signs)
        {
            if (s.area.intersects(player.getHitbox()))
            {
                FieldState state = plantSystem.getFieldState(s.fieldID);
                std::string plantName = plantSystem.getPlantName(s.fieldID);
                game.pushState(std::make_unique<MiniMenuState>(
                    s.fieldID, state, plantName,

                    // sadzenie
                    [this](int id) { 
                            std::string plantName = plantSystem.getPlantName(id);
                            std::string seedName = plantName + "_seed";

                            // sprawdzamnie czy są nasiona
                            if (plantSystem.storage[seedName] > 0)
                            {
                                plantSystem.storage[seedName]--;
                                plantSystem.plant(id);
                                return true;
                            }
                            return false;
                    },

                    // podlewanie
                    [this](int id) { plantSystem.water(id); },

                    // zbieranie
                    [this](int id) { plantSystem.harvest(id); }
                ));
                break;
            }
        }
    }

    // KAMERA I OGRANICZENIA MAPY
    float mapW = mapSizePixels.x;
    float mapH = mapSizePixels.y;

    player.sprite.setPosition(player.pos);
    camera.setCenter(player.pos);

    float halfW = camera.getSize().x / 2;
    float halfH = camera.getSize().y / 2;

    sf::Vector2f cam = camera.getCenter();

    if(cam.x < halfW) cam.x = halfW;
    if(cam.y < halfH) cam.y = halfH;
    if(cam.x > mapW - halfW) cam.x = mapW - halfW;
    if(cam.y > mapH - halfH) cam.y = mapH - halfH;

    camera.setCenter(cam);
    game.window.setView(camera);

    // AKTUALIZACJA LICZNIKA ZŁOTA
    goldText.setString("Zloto: " + std::to_string(gold));
}

// RYSOWANIE POJEDYNCZEJ WARSTWY KAFELKÓW KAMPY
void PlayState::drawTileLayer(const std::unique_ptr<tmx::Layer>& layer, sf::RenderWindow& window) {
    const tmx::TileLayer* tileLayer = dynamic_cast<const tmx::TileLayer*>(layer.get());
    if (!tileLayer) return;

    const auto& tiles = tileLayer->getTiles();
    auto tileSize = mapa.getTileSize();
    auto mapSize = mapa.getTileCount();

    for (unsigned y=0; y<mapSize.y; y++)
    {
        for (unsigned x=0; x<mapSize.x; x++)
        {
            std::size_t index = x + y * mapSize.x;
            if (index >= tiles.size())
                continue;

            const auto& tile = tiles[index];

            int gid = tile.ID;
            if (gid == 0)
                continue;

            // ustalenie tilesetu
            int tsIndex = -1;
            for (int i = 0; i < firstGIDs.size(); i++)
            {
                if (gid >= firstGIDs[i])
                    tsIndex = i;
                else
                    break;
            }

            if (tsIndex == -1)
                continue;

            const sf::Texture& tex = tilesetTextures[tsIndex];
            int localID = gid - firstGIDs[tsIndex];

            int columns = tex.getSize().x / tileSize.x;
            int tx = localID % columns;
            int ty = localID / columns;

            // rysowanie kafelka
            sf::Sprite sprite;
            sprite.setTexture(tex);
            sprite.setTextureRect(sf::IntRect(tx * tileSize.x, ty * tileSize.y, tileSize.x, tileSize.y));
            sprite.setPosition(x * tileSize.x, y * tileSize.y);

            window.draw(sprite);
        }
    }
}

// RYSOWANIE MAPY, GRACZA, ROŚLIN I INTERFEJSU
void PlayState::draw(Game& game) {
    sf::RenderWindow& window = game.window;
    window.setView(camera);

    const auto& layers = mapa.getLayers();

    // WARSTWY POD GRACZEM
    for (const auto& layer : layers)
    {
        std::string name = layer->getName();
        if (name == "ścieżka" || name == "podłoga")
            drawTileLayer(layer, window);
    }

    // GRACZ
    window.draw(player.sprite);

    // WARSTWY NAD GRACZEM
    for (const auto& layer : layers)
    {
        std::string name = layer->getName();
        if (name == "drzewa" || name == "skrzynka" || name == "tabliczki" || name == "sklep1" || name == "domki_atrapy" || name == "płoty" || name == "płoty_blokada")
            drawTileLayer(layer, window);
    }

    // RYSOWANIE ROŚLIN
    plantSystem.draw(game.window);

    // LICZNIKI CZASU NAD POLAMI
    for (const auto& s : signs)
    {
        float timeLeft = plantSystem.getTimeToNextStage(s.fieldID);
        if (timeLeft <= 0.f)
            continue;

        int seconds = (int)std::ceil(timeLeft);

        // pozycja w świecie
        sf::Vector2f pos(s.area.left + s.area.width / 2.f, s.area.top - 12.f);

        // ramka
        sf::RectangleShape frame;
        frame.setSize({34.f, 20.f});
        frame.setOrigin(frame.getSize().x / 2.f, frame.getSize().y / 2.f);
        frame.setPosition(pos);
        frame.setFillColor(sf::Color(0, 80, 0, 200));
        frame.setOutlineThickness(2);
        frame.setOutlineColor(sf::Color(0, 0, 0));

        // tekst
        sf::Text txt;
        txt.setFont(font);
        txt.setCharacterSize(14);
        txt.setFillColor(sf::Color::White);
        txt.setOutlineColor(sf::Color::Black);
        txt.setOutlineThickness(2);
        txt.setString(std::to_string(seconds) + "s");
        sf::FloatRect t = txt.getLocalBounds();
        txt.setOrigin(t.left + t.width / 2, t.top + t.height / 2); 
        txt.setPosition(pos);

        // skalowanie liczników zależnie od zoomu kamery
        float zoom = camera.getSize().x / 800.f;
        txt.setScale(1.f * zoom, 1.f * zoom);
        frame.setScale(1.f * zoom, 1.f * zoom);

        window.draw(frame);
        window.draw(txt);
    }

    // INTERFEJS ZŁOTA (rysowany w widoku ekranu)
    window.setView(game.window.getDefaultView());
    window.draw(goldBox);
    window.draw(goldText);
}