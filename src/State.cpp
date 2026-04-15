#include <SFML/Graphics.hpp>

class Game;

struct State {
    virtual ~State() = default;
    virtual void handleInput(Game& game) = 0;
    virtual void update(Game& game) = 0;
    virtual void render(Game& game, sf::RenderWindow& window) = 0;
};