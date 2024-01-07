#include "map.hpp"
#include <SFML/Graphics.hpp>
int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(1280, 960), "Tilemap");

    // define the level with an array of tile indices

    // create the tilemap from the level definition
    Map map;
    map.load();

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
        window.draw(map);
        window.display();
    }

    return 0;
}
