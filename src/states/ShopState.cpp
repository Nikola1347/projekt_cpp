#include "states/ShopState.hpp"
#include "core/Game.hpp"

// KONSTRUKTOR - TWORZY OPCJE SKLEPU I INTERFEJS
ShopState::ShopState(
    int& goldRef,
    PlantSystem& plantSystemRef
)
    : gold(goldRef),
      plantSystem(plantSystemRef)
{
    // tytuł
    font.loadFromFile("../assets/fonts/arial.ttf");
    title.setFont(font);
    title.setCharacterSize(26);
    title.setFillColor(sf::Color::Black);
    title.setString("Sklep");

    // ceny roślinek
    prices["truskawka"] = 2;
    prices["cebula"] = 4;
    prices["ziemniak"] = 6;
    prices["por"] = 8;

    // ceny sprzedaży roślinek
    sellPrices["truskawka"] = 3;
    sellPrices["cebula"] = 5;
    sellPrices["ziemniak"] = 7;
    sellPrices["por"] = 9;

    // opcja sprzedaży
    options.push_back("Sprzedaj wszystkie zbiory");

    // generowanie opcji kupna nasion
    for (auto& [plantName, def] : plantSystem.plantDefs)
    {
        options.push_back("Kup nasiona: " + plantName + " (" + std::to_string(prices[plantName]) + " zlota)");
    }

    // wyjście
    options.push_back("Wyjscie");

    // UI
    float titleHeight = 60.f;
    float optionHeight = 60.f;
    float maxWidth = 0.f;

    for (auto& text : options)
    {
        sf::Text t;
        t.setFont(font);
        t.setCharacterSize(24);
        t.setString(text);

        float w = t.getLocalBounds().width;
        if (w > maxWidth)
            maxWidth = w;
    }

    maxWidth += 200.f;

    // główna ramka
    box.setSize({ maxWidth, titleHeight + optionHeight * options.size()});
    box.setFillColor(sf::Color(0, 80, 0, 200));
    box.setOutlineColor(sf::Color::Black);
    box.setOutlineThickness(4);

    // tworzenie opcji UI
    for (int i=0; i<(int)options.size(); i++)
    {
        sf::Text t;
        t.setFont(font);
        t.setString(options[i]);
        t.setCharacterSize(24);
        t.setFillColor(sf::Color::Black);

        sf::RectangleShape r;
        r.setSize({maxWidth-40.f, 40});
        r.setFillColor(sf::Color(0, 120, 0));
        r.setOutlineColor(sf::Color::Black);
        r.setOutlineThickness(3);

        boxes.push_back(r);
        texts.push_back(t);
    }
}

// OBSŁUGA WEJŚCIA UŻYTKOWNIKA
void ShopState::handleInput(Game& game)
{
    sf::Event event;
    while (game.window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            game.window.close();

        if (event.type == sf::Event::KeyPressed)
        {
            // wyjście
            if (event.key.code == sf::Keyboard::Escape)
            {
                game.popState();
                return;
            }

            // nawigacja
            if (event.key.code == sf::Keyboard::Down)
                selected = (selected + 1) % (int)options.size();

            if (event.key.code == sf::Keyboard::Up)
                selected = (selected - 1 + (int)options.size()) % (int)options.size();

            // wybór opcji
            if (event.key.code == sf::Keyboard::Enter)
            {
                const std::string& opt = options[selected];

                // SPRZEDAŻ WSZYSTKICH ZBIORÓW
                if (opt == "Sprzedaj wszystkie zbiory")
                {
                    for (auto& [name, amount] : plantSystem.storage)
                    {
                        if (amount > 0)
                        {
                            int earned = amount * sellPrices[name];
                            gold += earned;
                            amount = 0;
                        }
                    }

                    game.popState();
                    return;
                }

                // KUPNO NASION
                if (opt.rfind("Kup nasiona: ", 0) == 0)
                {
                    std::string plantName = opt.substr(strlen("Kup nasiona: "));
                    plantName = plantName.substr(0, plantName.find(" ("));

                    int cost = prices[plantName];

                    if (gold >= cost)
                    {
                        gold -= cost;
                        plantSystem.storage[plantName + "_seed"]++;
                    }

                    game.popState();
                    return;
                }

                // WYJŚCIE
                if (opt == "Wyjscie")
                {
                    game.popState();
                    return;
                }
            }
        }
    }
}

// AKTUALIZACJA LOGIKI SKLEPU
void ShopState::update(Game& game)
{
    for (int i=0; i<boxes.size(); i++)
    {
        if (i == selected)
            boxes[i].setFillColor(sf::Color(0, 160, 0));
        else
            boxes[i].setFillColor(sf::Color(0, 120, 0));
    }
}

// RYSOWANIE ELEMENTÓW SKLEPU
void ShopState::draw(Game& game)
{
    game.window.setView(game.window.getDefaultView());

    float titleHeight = 60.f;
    float optionHeight = 60.f;

    // wycentrowanie głównej ramki
    float cx = game.window.getSize().x / 2.f;
    float cy = game.window.getSize().y / 2.f;
    box.setPosition(cx - box.getSize().x / 2.f, cy - box.getSize().y / 2.f);

    // wycentrowanie tytułu
    sf::FloatRect tb = title.getLocalBounds();
    title.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
    title.setPosition(box.getPosition().x + box.getSize().x / 2.f, box.getPosition().y + 30.f);

    // pozycje opcji
    for (int i=0; i<(int)boxes.size(); i++)
    {
        boxes[i].setPosition(box.getPosition().x + (box.getSize().x - boxes[i].getSize().x) / 2.f, box.getPosition().y + titleHeight + i * optionHeight);

        sf::FloatRect bounds = texts[i].getLocalBounds();
        texts[i].setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
        texts[i].setPosition(boxes[i].getPosition().x + boxes[i].getSize().x / 2.f, boxes[i].getPosition().y + boxes[i].getSize().y / 2.f);
    }

    // rysowanie
    game.window.draw(box);
    game.window.draw(title);

    for (int i=0; i<boxes.size(); i++)
    {
        game.window.draw(boxes[i]);
        game.window.draw(texts[i]);
    }
}