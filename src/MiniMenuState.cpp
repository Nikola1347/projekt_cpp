#include "MiniMenuState.hpp"
#include "Game.hpp"
#include <iostream>

MiniMenuState::MiniMenuState(
    int id,
    FieldState state,
    const std::string& plantName,
    std::function<void(int)> onPlant,
    std::function<void(int)> onWater,
    std::function<void(int)> onHarvest
)
    : fieldID(id),
      plantName(plantName),
      fieldState(state),
      onPlant(onPlant),
      onWater(onWater),
      onHarvest(onHarvest)
{
    // opcje zależne od stanu pola
    switch (fieldState)
    {
        case FieldState::Empty:
            options = { "Zasadz", "Wyjscie" };
            break;
        case FieldState::NeedsWater:
            options = { "Podlej", "Wyjscie" };
            break;
        case FieldState::Growing:
            options = { "Wyjscie" };
            break;
        case FieldState::ReadyToHarvest:
            options = { "Zbierz plony", "Wyjscie" };
            break;
    }

    //wizualne
    font.loadFromFile("assets/fonts/arial.ttf");
    float titleHeight = 60.f;
    float optionHeight = 60.f;
    box.setSize({
        260.f,
        titleHeight + optionHeight * options.size()
    });
    box.setFillColor(sf::Color(0, 80, 0, 200));
    box.setOutlineColor(sf::Color::Black);
    box.setOutlineThickness(4);

    title.setFont(font);
    title.setCharacterSize(26);
    title.setFillColor(sf::Color::Black);
    title.setString(plantName);

    for (int i=0; i<(int)options.size(); i++)
    {
        sf::Text t;
        t.setFont(font);
        t.setString(options[i]);
        t.setCharacterSize(24);
        t.setFillColor(sf::Color::Black);

        sf::RectangleShape r;
        r.setSize({220, 40});
        r.setFillColor(sf::Color(0, 120, 0));
        r.setOutlineColor(sf::Color::Black);
        r.setOutlineThickness(3);

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
                selected = (selected + 1) % (int)options.size();
 
            if (event.key.code == sf::Keyboard::Up)
                selected = (selected - 1 + (int)options.size()) % (int)options.size();
 
            if (event.key.code == sf::Keyboard::Enter)
            {
                const std::string& opt = options[selected];
 
                if (opt == "Zasadz")
                {
                    if (onPlant) onPlant(fieldID);
                    game.popState();
                }
                else if (opt == "Podlej")
                {
                    if (onWater) onWater(fieldID);
                    game.popState();
                }
                else if (opt == "Zbierz plony")
                {
                    if (onHarvest) onHarvest(fieldID);
                    game.popState();
                }
                else if (opt == "Wyjscie")
                {
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

    float titleHeight = 60.f;
    float optionHeight = 60.f;

    // dynamiczny rozmiar
    box.setSize({
        260.f,
        titleHeight + optionHeight * options.size()
    });

    // wycentrowanie boxa
    float cx = game.window.getSize().x / 2.f;
    float cy = game.window.getSize().y / 2.f;
    box.setPosition(cx - box.getSize().x / 2.f, cy - box.getSize().y / 2.f);

    // wyśrodkowanie tytułu
    sf::FloatRect tb = title.getLocalBounds();
    title.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
    title.setPosition(
        box.getPosition().x + box.getSize().x / 2.f,
        box.getPosition().y + 30.f
    );

    // pozycje opcji
    for (int i=0; i<(int)boxes.size(); i++)
    {
        boxes[i].setPosition(
            box.getPosition().x + 20.f,
            box.getPosition().y + titleHeight + i * optionHeight
        );

        sf::FloatRect bounds = texts[i].getLocalBounds();
        texts[i].setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
        texts[i].setPosition(
            boxes[i].getPosition().x + boxes[i].getSize().x / 2.f,
            boxes[i].getPosition().y + boxes[i].getSize().y / 2.f
        );
    }

    // rysowanie
    game.window.draw(box);
    game.window.draw(title);

    for (int i=0; i<boxes.size(); i++) {
        game.window.draw(boxes[i]);
        game.window.draw(texts[i]);
    }
}