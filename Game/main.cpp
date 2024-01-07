#include "map.hpp"
#include "soldier.hpp"
#include <SFML/Graphics.hpp>
int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(1280, 960), "Tilemap");

    // define the level with an array of tile indices

    // create the tilemap from the level definition
    Map* map = Map::getInstance();
    map->load();
    Soldier_entity* soldiers = new Soldier_entity();
    soldiers->addSoldier(INFANTRY, sf::Vector2u(0, 0), right);
    soldiers->addSoldier(KNIGHT, sf::Vector2u(1, 0), down);
    soldiers->addSoldier(ARCHER, sf::Vector2u(2, 0), left);
    soldiers->addSoldier(CASTER, sf::Vector2u(3, 0), up);
    soldiers->addSoldier(BERSERKER, sf::Vector2u(4, 0), right);
    // run the main loop
    while (window.isOpen()) {
        // handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // draw the map
        window.clear();
        soldiers->update();
        window.draw(*map);
        window.draw(*soldiers);
        window.display();
    }

    return 0;
}
