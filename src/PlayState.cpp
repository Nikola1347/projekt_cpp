#include "PlayState.hpp"
#include "Game.hpp"

PlayState::PlayState() {
    font.loadFromFile("assets/fonts/arial.ttf");
    text.setFont(font);
    text.setString("GRA");
    text.setCharacterSize(32);
    text.setFillColor(sf::Color::Yellow);
    text.setPosition(200, 250);
}

void PlayState::handleInput(Game& game) {
    sf::Event event;
    while(game.window.pollEvent(event)) {
        if(event.type == sf::Event::Closed)
            game.window.close();
    }
}

void PlayState::update(Game& game) {}

void PlayState::draw(Game& game) {
    game.window.draw(text);
}