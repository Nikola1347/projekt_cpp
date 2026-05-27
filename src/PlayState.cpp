#include "PlayState.hpp"
#include "Game.hpp"
#include "PauseState.hpp"
#include "MiniMenuState.hpp"
#include "../systems/PlantSystem.hpp"
#include <iostream>
#include <cmath>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>

PlayState::PlayState()
{
    // wczytywanie mapy
    if (!mapa.load("assets/maps/mapa.tmx"))
        std::cout << "Błąd: mapa.tmx" << std::endl;

    // ładowanie tilesetów
    const auto& tilesets = mapa.getTilesets();
    for (const auto& ts : tilesets)
    {
        std::cout << "Tileset path: " << ts.getImagePath() << std::endl;
    }

    if (!font.loadFromFile("assets/fonts/arial.ttf"))
    {
        std::cout << "błąd fontu w PlayState\n";
    }

    // ładowanie tilesetów
    for (const auto& ts : tilesets)
    {
        sf::Texture tex;
        std::string path = ts.getImagePath();

        if (!tex.loadFromFile(path))
            std::cout << "Blad ladowania tilesetu: " << path << std::endl;

        tilesetTextures.push_back(std::move(tex));
        firstGIDs.push_back(ts.getFirstGID());
    }

    // wczytywanie kolizji (z tilesetu)
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
    player.pos = { 730.f, 500.f }; //zmień tego spawna
    player.sprite.setPosition(player.pos);

    // mapa
    auto tile = mapa.getTileSize();
    auto count = mapa.getTileCount();
    mapSizePixels.x = tile.x * count.x;
    mapSizePixels.y = tile.y * count.y;

    // kamera
    camera.setSize(400, 300);
    camera.setCenter(400, 300);

    // PlantSystem
    plantSystem = PlantSystem();
    plantSystem.load(mapa);

    // wczytywanie tabliczek z warstwy tab_obj
    for (const auto& layer : mapa.getLayers())
    {
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
    }
}

void PlayState::handleInput(Game& game) {
    sf::Event event;
    while (game.window.pollEvent(event)) {

        if (event.type == sf::Event::Closed)
            game.window.close();

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::M) {
                game.pushState(std::make_unique<PauseState>());
            }
        }    
    }
}

// sprawdzanie kolizii
bool PlayState::checkCollision(const sf::FloatRect& nextPos)
{
    for (const auto& rect : collisionRects)
    {
        if (rect.intersects(nextPos))
            return true;
    }
    return false;
}

void PlayState::update(Game& game) {

    collisionRects.clear();

    const auto& layers = mapa.getLayers();
    auto tileSize = mapa.getTileSize();
    auto mapSize = mapa.getTileCount();

    for (const auto& layer : layers)
    {
        //kolizje
        bool layerHasCollision = false;

        for (const auto& p : layer->getProperties())
        {
            if (p.getName() == "collision" && p.getBoolValue())
                layerHasCollision = true;
        }

        const tmx::TileLayer* tileLayer = dynamic_cast<const tmx::TileLayer*>(layer.get());
        if (!tileLayer)
            continue;

        const auto& tiles = tileLayer->getTiles();

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

                // sprawdzanie czy ma kolizje
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

    //poruszanie się
    float dt = dtClock.restart().asSeconds();
    float speed = 100.f; //do zmiany
    player.walking = false;

    //góra
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

    //dół
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

    //lewo
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

    //prawo
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

    player.update(dt);
    plantSystem.update(dt);

    // wywołanie mini menu
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
                    [this](int id) { plantSystem.plant(id); },
                    [this](int id) { plantSystem.water(id); },
                    [this](int id) { plantSystem.harvest(id); }
                ));
                break;
            }
        }
    }

    //mapa
    float mapW = mapSizePixels.x;
    float mapH = mapSizePixels.y;

    //ustawienie kamery
    player.sprite.setPosition(player.pos);
    camera.setCenter(player.pos);

    float halfW = camera.getSize().x / 2;
    float halfH = camera.getSize().y / 2;

    sf::Vector2f cam = camera.getCenter();

    //ograniczenie dla kamery
    if(cam.x < halfW) cam.x = halfW;
    if(cam.y < halfH) cam.y = halfH;
    if(cam.x > mapW - halfW) cam.x = mapW - halfW;
    if(cam.y > mapH - halfH) cam.y = mapH - halfH;

    camera.setCenter(cam);
    game.window.setView(camera);
}

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

void PlayState::draw(Game& game) {
    sf::RenderWindow& window = game.window;
    window.setView(camera);

    const auto& layers = mapa.getLayers();

    // warstwy pod
    for (const auto& layer : layers)
    {
        std::string name = layer->getName();
        if (name == "ścieżka" || name == "podłoga")
        {
            drawTileLayer(layer, window);
        }
    }

    // gracz
    window.draw(player.sprite);

    // warstwy nad
    for (const auto& layer : layers)
    {
        std::string name = layer->getName();

        if (name == "drzewa" || name == "skrzynka" || name == "tabliczki" || name == "sklep1" || name == "domki_atrapy" || name == "płoty" || name == "płoty_blokada")
        {
            drawTileLayer(layer, window);
        }
    }

    plantSystem.draw(game.window);

    // timer nad roślinami
    for (const auto& s : signs)
    {
        float timeLeft = plantSystem.getTimeToNextStage(s.fieldID);
        if (timeLeft <= 0.f)
            continue;

        int seconds = (int)std::ceil(timeLeft);

        // pozycja w świecie
        sf::Vector2f pos(s.area.left, s.area.top - 22.f);

        // ramka
        sf::RectangleShape frame;
        frame.setSize({34.f, 20.f});
        frame.setPosition(pos.x - 4.f, pos.y - 2.f);
        frame.setFillColor(sf::Color(0, 80, 0, 200));
        frame.setOutlineThickness(2);
        frame.setOutlineColor(sf::Color(0, 0, 0));

        // tekst
        sf::Text txt;
        txt.setFont(font);
        txt.setCharacterSize(14);
        txt.setFillColor(sf::Color::White);
        txt.setString(std::to_string(seconds) + "s");

        // lekko wyśrodkowany tekst
        txt.setPosition(pos.x + 4.f, pos.y + 1.f);

        window.draw(frame);
        window.draw(txt);
    }

    // DEBUGI
    sf::FloatRect hb = player.getHitbox();
    sf::RectangleShape r;
    r.setPosition(hb.left, hb.top);
    r.setSize({hb.width, hb.height});
    r.setFillColor(sf::Color(0, 255, 0, 120));
    window.draw(r);

    for (const auto& rect : collisionRects)
    {
        sf::RectangleShape r;
        r.setPosition(rect.left, rect.top);
        r.setSize({rect.width, rect.height});
        r.setFillColor(sf::Color(255, 0, 0, 100));
        window.draw(r);
    }
}