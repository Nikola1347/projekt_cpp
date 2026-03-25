#include "HelpState.hpp"
#include "Game.hpp"

HelpState::HelpState() {
    font.loadFromFile("assets/fonts/arial.ttf");
    text.setFont(font);
    text.setString("Pomoc:\nStrzalki - poruszanie\nEnter - wybierz\nESC - powrot");
    text.setCharacterSize(28);
    text.setFillColor(sf::Color::White);
    text.setPosition(50, 100);
}

void HelpState::handleInput(Game& game) {
    sf::Event event;
    while (game.window.pollEvent(event)) {
        if(event.type == sf::Event::Closed)
            game.window.close();

        if(event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Escape) {
            game.popState();
        }
    }
}

void HelpState::update(Game& game) {}

void HelpState::draw(Game& game) {
    game.window.draw(text);
}