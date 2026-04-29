#include "PlayState.hpp"
#include "Game.hpp"
#include "PauseState.hpp"
#include <iostream>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>

PlayState::PlayState()
{
    if (!mapa.load("assets/maps/mapa.tmx"))
        std::cout << "Błąd: mapa.tmx" << std::endl;

    // test do ścieżek (USUŃ PÓŹNIEJ)
    const auto& tilesets = mapa.getTilesets();
    for (const auto& ts : tilesets)
    {
        std::cout << "Tileset path: " << ts.getImagePath() << std::endl;
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

    // mapa
    auto tile = mapa.getTileSize();
    auto count = mapa.getTileCount();
    mapSizePixels.x = tile.x * count.x;
    mapSizePixels.y = tile.y * count.y;

    // kamera
    camera.setSize(400, 300);
    camera.setCenter(400, 300);
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

void PlayState::update(Game& game) {

    //poruszanie się
    float dt = dtClock.restart().asSeconds();
    float speed = 150.f; //do zmiany

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        player.pos.y -= speed * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        player.pos.y += speed * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        player.pos.x -= speed * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        player.pos.x += speed * dt;

    //mapa
    float mapW = mapSizePixels.x;
    float mapH = mapSizePixels.y;

    //bariera nie wychodzenia za mape (do zmiany/usunięcia)
    if (player.pos.x < 0) player.pos.x = 0;
    if (player.pos.y < 0) player.pos.y = 0;
    if (player.pos.x > mapW) player.pos.x = mapW;
    if (player.pos.y > mapH) player.pos.y = mapH;

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

void PlayState::draw(Game& game)
{
    sf::RenderWindow& window = game.window;
    window.setView(camera);

    const auto& layers = mapa.getLayers();

    for (const auto& layer : layers)
    {
        if (layer->getType() != tmx::Layer::Type::Tile)
            continue;

        // sprawdzanie w czasie programu czy wskaźnik lub referencja do klasy bazowej faktycznie wskazuje na obiekt klasy pochodnej
        const tmx::TileLayer* tileLayer = dynamic_cast<const tmx::TileLayer*>(layer.get());
        if (!tileLayer)
            continue;

        const auto& tiles = tileLayer->getTiles();
        auto tileSize = mapa.getTileSize();
        auto mapSize = mapa.getTileCount();

        for (unsigned y=0; y<mapSize.y; y++)
        {
            for (unsigned x=0; x<mapSize.x; x++)
            {
                std::size_t index = x + y * mapSize.x;
                const auto& tile = tiles[index];

                int gid = tile.ID;
                if (gid == 0)
                    continue;

                // znalezienie tilesetu dla GID
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

                const sf::Texture& tex = tilesetTextures[tsIndex];
                int localID = gid - firstGIDs[tsIndex];

                int columns = tex.getSize().x / tileSize.x;

                int tx = localID % columns;
                int ty = localID / columns;

                sf::Sprite sprite;
                sprite.setTexture(tex);
                sprite.setTextureRect(sf::IntRect(tx * tileSize.x, ty * tileSize.y, tileSize.x, tileSize.y));
                sprite.setPosition(x * tileSize.x, y * tileSize.y);
                window.draw(sprite);
            }
        }
    }
    window.draw(player.sprite);
}