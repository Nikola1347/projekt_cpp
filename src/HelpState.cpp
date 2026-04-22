#include "HelpState.hpp"
#include "Game.hpp"

HelpState::HelpState() {
    font.loadFromFile("assets/fonts/arial.ttf");

    // wizualne
    background.setSize({800, 600});
    background.setFillColor(sf::Color(0, 80, 0));

    title.setFont(font);
    title.setString("Pomoc");
    title.setCharacterSize(48);
    title.setFillColor(sf::Color::Black);

    float titleWidth = title.getLocalBounds().width;
    title.setPosition(400 - titleWidth / 2, 60);

    box.setSize({600, 350});
    box.setFillColor(sf::Color(0, 120, 0));
    box.setOutlineColor(sf::Color::Black);
    box.setOutlineThickness(4);
    box.setPosition(100, 150);

    text.setFont(font);
    text.setCharacterSize(32);
    text.setFillColor(sf::Color::Black);
    text.setString(
        "Sterowanie:\n"
        "W - gora\n"
        "S - dol\n"
        "A - lewo\n"
        "D - prawo\n"
        "ESC - wroc"
    );

    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width /2, bounds.top + bounds.height /2);
    text.setPosition(box.getPosition().x + box.getSize().x /2, box.getPosition().y + box.getSize().y /2);
}

void HelpState::handleInput(Game& game) {
    sf::Event event;
    while (game.window.pollEvent(event)) {

        if (event.type == sf::Event::Closed)
            game.window.close();

        if (event.type == sf::Event::KeyPressed) {
            if(event.key.code == sf::Keyboard::Escape)
            {
                game.popState();
                return;
            }
        }
    }
}

void HelpState::update(Game& game) {
}

void HelpState::draw(Game& game) {
    game.window.setView(game.window.getDefaultView());
    game.window.draw(background);
    game.window.draw(title);
    game.window.draw(box);
    game.window.draw(text);
}