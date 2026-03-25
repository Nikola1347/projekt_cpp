#include "MenuState.hpp"
#include "Game.hpp"
#include "PlayState.hpp"
#include "HelpState.hpp"
#include <iostream>

MenuState::MenuState() {
    font.loadFromFile("assets/fonts/arial.ttf");

    std::vector<std::string> names = {
        "Nowa gra",
        "Wczytaj gre",
        "Pomoc",
        "Wyjscie"
    };

    for(int i=0; i<names.size(); i++) {
        sf::Text text;
        text.setFont(font);
        text.setString(names[i]);
        text.setCharacterSize(32);
        text.setFillColor(i == 0 ? sf::Color::Yellow : sf::Color::White);
        text.setPosition(100, 150+i*60);
        options.push_back(text);
    }
}

void MenuState::handleInput(Game& game) {
    sf::Event event;
    while(game.window.pollEvent(event)) {
        if(event.type == sf::Event::Closed)
            game.window.close();

        if(event.type == sf::Event::KeyPressed) {

            if(event.key.code == sf::Keyboard::Up) {
                options[selected].setFillColor(sf::Color::White);
                selected = (selected - 1 + options.size()) % options.size();
                options[selected].setFillColor(sf::Color::Yellow);
            }

            if(event.key.code == sf::Keyboard::Down) {
                options[selected].setFillColor(sf::Color::White);
                selected = (selected + 1) % options.size();
                options[selected].setFillColor(sf::Color::Yellow);
            }

            if(event.key.code == sf::Keyboard::Enter) {
                if(selected == 0) {
                    game.pushState(std::make_unique<PlayState>());
                }
                else if(selected == 1) {
                    std::cout << "wczytywanie gry (do dodania)" << std::endl;
                }
                else if(selected == 2) {
                    game.pushState(std::make_unique<HelpState>());
                }
                else if(selected == 3) {
                    game.window.close();
                }
            }
        }
    }
}

void MenuState::update(Game& game) {}

void MenuState::draw(Game& game) {
    for(auto& t : options)
        game.window.draw(t);
}
