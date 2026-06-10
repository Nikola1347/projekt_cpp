#include "states/PauseState.hpp"
#include "core/Game.hpp"
#include "states/HelpState.hpp"
#include "states/MenuState.hpp"

// KONSTRUKTOR - TWORZY WYGLĄD MENU PAUZY
PauseState::PauseState() {
    font.loadFromFile("../assets/fonts/arial.ttf");

    // tło
    background.setSize({800, 600});
    background.setFillColor(sf::Color(0, 80, 0));

    // tytuł
    title.setFont(font);
    title.setString("Pauza");
    title.setCharacterSize(48);
    title.setFillColor(sf::Color::Black);
    float titleWidth = title.getLocalBounds().width;
    title.setPosition(400 - titleWidth / 2, 80);

    // nazwy opcji
    std::vector<std::string> names = {
        "Pomoc",
        "Zapisz gre",
        "Powrot do menu",
        "Wroc do gry"
    };

    float centerX = 800 / 2;
    float startY = 200;

    // tworzenie opcji
    for (int i=0; i<names.size(); i++) {

        sf::Text text;
        text.setFont(font);
        text.setString(names[i]);
        text.setCharacterSize(32);
        text.setFillColor(sf::Color::Black);

        sf::RectangleShape box;
        box.setSize({400, 50});
        box.setFillColor(sf::Color(0, 120, 0));
        box.setOutlineColor(sf::Color::Black);
        box.setOutlineThickness(3);

        float y = startY + i*80;
        box.setPosition(centerX - 200, y);

        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.left + bounds.width /2, bounds.top + bounds.height /2);
        text.setPosition(box.getPosition().x + box.getSize().x /2, box.getPosition().y + box.getSize().y /2);

        boxes.push_back(box);
        texts.push_back(text);
    }
}

// OBSŁUGA WEJŚCIA UŻYTKOWNIKA
void PauseState::handleInput(Game& game) {
    sf::Event event;
    while (game.window.pollEvent(event)) {

        if (event.type == sf::Event::Closed)
            game.window.close();

        if (event.type == sf::Event::KeyPressed) {

            if (event.key.code == sf::Keyboard::Down)
                selected = (selected + 1) % texts.size();

            if (event.key.code == sf::Keyboard::Up)
                selected = (selected - 1 + texts.size()) % texts.size();

            if (event.key.code == sf::Keyboard::Enter) {

                // pomoc
                if (selected == 0) {
                    game.pushState(std::make_unique<HelpState>());
                    return;
                }

                // zapis gry
                if (selected == 1) {
                    PlayState* play = dynamic_cast<PlayState*>(game.getStateBelowTop());
                    if (play) {
                        SaveSystem::save(*play);
                        texts[1].setString("Zapisano");
                    }
                    else {
                        texts[1].setString("Błąd zapisu");
                    }
                    return;
                }

                // powrót do menu
                if (selected == 2) {
                    game.goToMenu();
                    return;
                }

                // powrót do gry
                if (selected == 3) {
                    if (auto play = game.getPlayState()) {
                        play->paused = false;
                        play->resetClock();
                    }
                    game.popState();
                    return;
                }
            }

            // szybkie wyjście z pauzy
            if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::M) {
                if (auto play = game.getPlayState()) {
                        play->paused = false;
                        play->resetClock();
                    }
                game.popState();
                return;
            }
        }
    }
}

// AKTUALIZACJA LOGIKI MENU PAUZY
void PauseState::update(Game& game) {
    for (int i=0; i<texts.size(); i++) {
        if (i == selected)
            boxes[i].setFillColor(sf::Color(0, 160, 0));
        else
            boxes[i].setFillColor(sf::Color(0, 120, 0));
    }
}

// RYSOWANIE ELEMENTÓW MENU PAUZY
void PauseState::draw(Game& game) {
    game.window.setView(game.window.getDefaultView());
    game.window.draw(background);
    game.window.draw(title);

    for (int i=0; i<texts.size(); i++) {
        game.window.draw(boxes[i]);
        game.window.draw(texts[i]);
    }
}