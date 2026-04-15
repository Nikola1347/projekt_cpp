#include "PlayState.hpp"
#include "Game.hpp"
#include "PauseState.hpp"

PlayState::PlayState() {
    font.loadFromFile("assets/fonts/arial.ttf");
    text.setFont(font);
    text.setString("GRA");
    text.setCharacterSize(32);
    text.setFillColor(sf::Color::Yellow);
    text.setPosition(200, 250);
    mapTexture.loadFromFile("assets/maps/test_mapa.png");
    mapSprite.setTexture(mapTexture);
    mapSprite.setPosition(0, 0);
    camera.setSize(800, 600);
    camera.setCenter(player.pos);
}

void PlayState::handleInput(Game& game) {
    sf::Event event;
    while(game.window.pollEvent(event)) {
        if(event.type == sf::Event::Closed)
            game.window.close();
        if(event.type == sf::Event::KeyPressed) {
            if(event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::M) {
                game.pushState(std::make_unique<PauseState>());
            }
        }    
    }
}

void PlayState::update(Game& game) {

    float dt = dtClock.restart().asSeconds();
    float speed = 150.f;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        player.pos.y -= speed * dt;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        player.pos.y += speed * dt;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        player.pos.x -= speed * dt;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        player.pos.x += speed * dt;

    float mapW = mapTexture.getSize().x;
    float mapH = mapTexture.getSize().y;

    if (player.pos.x < 0) player.pos.x = 0;
    if (player.pos.y < 0) player.pos.y = 0;
    if (player.pos.x > mapW) player.pos.x = mapW;
    if (player.pos.y > mapH) player.pos.y = mapH;

    player.sprite.setPosition(player.pos);

    camera.setCenter(player.pos);

    float halfW = camera.getSize().x / 2;
    float halfH = camera.getSize().y / 2;

    sf::Vector2f cam = camera.getCenter();

    if (cam.x < halfW) cam.x = halfW;
    if (cam.y < halfH) cam.y = halfH;
    if (cam.x > mapW - halfW) cam.x = mapW - halfW;
    if (cam.y > mapH - halfH) cam.y = mapH - halfH;

    camera.setCenter(cam);
    game.window.setView(camera);
}

void PlayState::draw(Game& game) {
    game.window.draw(text);
    game.window.draw(mapSprite);
    game.window.draw(player.sprite);
    game.window.setView(game.window.getDefaultView());
}