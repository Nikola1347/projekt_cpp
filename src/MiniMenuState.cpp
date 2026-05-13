#include "MiniMenuState.hpp"
#include "Game.hpp"
#include <iostream>

MiniMenuState::MiniMenuState()
{
    //wizualne
    font.loadFromFile("assets/fonts/arial.ttf");
    box.setSize({260, 300});
    box.setFillColor(sf::Color(0, 80, 0, 200));
    box.setOutlineColor(sf::Color::Black);
    box.setOutlineThickness(4);
    box.setPosition(
        800/2 - box.getSize().x/2,
        600/2 - box.getSize().y/2
    );

    std::vector<std::string> names = {
        "Kup",
        "Podlej",
        "Zbierz plony",
        "Wyjscie"
    };

    float centerX = box.getPosition().x + box.getSize().x / 2;
    float startY  = box.getPosition().y + 50;

    for (int i=0; i<names.size(); i++)
    {
        sf::Text t;
        t.setFont(font);
        t.setString(names[i]);
        t.setCharacterSize(24);
        t.setFillColor(sf::Color::Black);

        sf::RectangleShape r;
        r.setSize({220, 40});
        r.setFillColor(sf::Color(0, 120, 0));
        r.setOutlineColor(sf::Color::Black);
        r.setOutlineThickness(3);

        float y = startY + i*60;
        r.setPosition(centerX - 110, y);

        sf::FloatRect bounds = t.getLocalBounds();
        t.setOrigin(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
        t.setPosition(r.getPosition().x + r.getSize().x / 2, r.getPosition().y + r.getSize().y / 2);

        boxes.push_back(r);
        texts.push_back(t);
    }
}

void MiniMenuState::handleInput(Game& game)
{
    sf::Event event;
    while (game.window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            game.window.close();

        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
            {
                game.popState();
                return;
            }

            if (event.key.code == sf::Keyboard::Down)
                selected = (selected + 1) % texts.size();

            if (event.key.code == sf::Keyboard::Up)
                selected = (selected - 1 + texts.size()) % texts.size();

            if (event.key.code == sf::Keyboard::Enter)
            {
                if (event.key.code == sf::Keyboard::Enter) { //dodaj działanie
                    if (selected == 0)
                    {
                        std::cout << "kup\n";
                    }
                    if (selected == 1)
                    {
                        std::cout << "podlej\n";
                    }
                    if (selected == 2)
                    {
                        std::cout << "zbierz plony\n";
                    }
                    if (selected == 3)
                        game.popState();
                }
            }
        }
    }
}

void MiniMenuState::update(Game& game) {
    for (int i=0; i<boxes.size(); i++) {
        if (i == selected)
            boxes[i].setFillColor(sf::Color(0, 160, 0));
        else
            boxes[i].setFillColor(sf::Color(0, 120, 0));
    }
}

void MiniMenuState::draw(Game& game) {
    game.window.setView(game.window.getDefaultView());

    // ustawianie na środku
    float cx = game.window.getSize().x / 2.f;
    float cy = game.window.getSize().y / 2.f;
    box.setPosition(cx - box.getSize().x / 2.f, cy - box.getSize().y / 2.f);

    // opcje względem boxa
    for (int i=0; i<boxes.size(); i++) {
        boxes[i].setPosition(
            box.getPosition().x + 20,
            box.getPosition().y + 40 + i*60
        );

        texts[i].setPosition(
            boxes[i].getPosition().x + boxes[i].getSize().x / 2.f,
            boxes[i].getPosition().y + boxes[i].getSize().y / 2.f
        );
    }

    // rysowanie
    game.window.draw(box);

    for (int i=0; i<boxes.size(); i++) {
        game.window.draw(boxes[i]);
        game.window.draw(texts[i]);
    }
}