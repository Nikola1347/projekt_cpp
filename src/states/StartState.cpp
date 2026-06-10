#include "states/StartState.hpp"
#include "core/Game.hpp"
#include "states/PlayState.hpp"

// KONSTRUKTOR – TWORZY EKRAN POWITALNY Z INSTRUKCJĄ
StartState::StartState() {
    font.loadFromFile("../assets/fonts/arial.ttf");

    // tło
    background.setSize({800, 600});
    background.setFillColor(sf::Color(0, 80, 0));

    // tytuł
    title.setFont(font);
    title.setString("Witaj w grze!");
    title.setCharacterSize(48);
    title.setFillColor(sf::Color::Black);

    float titleWidth = title.getLocalBounds().width;
    title.setPosition(400 - titleWidth / 2, 40);

    // ramka
    box.setSize({750, 400});
    box.setFillColor(sf::Color(0, 120, 0));
    box.setOutlineColor(sf::Color::Black);
    box.setOutlineThickness(4);
    box.setPosition(25, 120);

    // opis gry
    text.setFont(font);
    text.setCharacterSize(28);
    text.setFillColor(sf::Color::Black);
    text.setString(
        "Twoim zadaniem jest hodowla roslin na polach,\n"
        "podlewanie ich i zbieranie plonow.\n"
        "Aby zasadzic rosliny, kup nasiona w sklepie, ktory\n"
        "znajdziesz idac sciezka w gore. W sklepie mozesz tez\n"
        "sprzedac plony. Nie zapomnij podlewac roslin\n"
        "od razu po zasadzeniu.\n"
        "\n"
        "Sterowanie:\n"
        "WASD/strzalki - ruch\n"
        "E - interakcja\n"
        "ESC - pauza"
    );

    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width /2, bounds.top + bounds.height /2);
    text.setPosition(box.getPosition().x + box.getSize().x /2, box.getPosition().y + box.getSize().y /2 - 20);

    // napis na dole
    continueText.setFont(font);
    continueText.setCharacterSize(26);
    continueText.setFillColor(sf::Color::Yellow);
    continueText.setString("Wcisnij [E], aby kontynuowac");

    sf::FloatRect cb = continueText.getLocalBounds();
    continueText.setOrigin(cb.left + cb.width /2, cb.top + cb.height /2);
    continueText.setPosition(400, 550);
}

// OBSŁUGA WEJŚCIA UŻYTKOWNIKA
void StartState::handleInput(Game& game) {
    sf::Event event;
    while (game.window.pollEvent(event)) {

        if (event.type == sf::Event::Closed)
            game.window.close();

        if (event.type == sf::Event::KeyPressed) {
            if(event.key.code == sf::Keyboard::E)
            {
                game.pushState(std::make_unique<PlayState>());
                return;
            }
        }
    }
}

// AKTUALIZACJA LOGIKI EKRANU STARTOWEGO
void StartState::update(Game& game) {
    // brak logiki do aktualizacji
}

// RYSOWANIE ELEMENTÓW EKRANU STARTOWEGO
void StartState::draw(Game& game) {
    game.window.setView(game.window.getDefaultView());
    game.window.draw(background);
    game.window.draw(title);
    game.window.draw(box);
    game.window.draw(text);
    game.window.draw(continueText);
}