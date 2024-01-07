#pragma once
#include "map.hpp"
#include "soldier.hpp"
class Game {
private:
    Map* map;
    Soldier_entity *blue_soldier, *red_soldier;
    sf::Window window;
    sf::Thread render_thread;

public:
    Game()
        : window(sf::Window(sf::VideoMode(1280, 960), "Tilemap"))
        , blue_soldier(new Soldier_entity(1))
        , red_soldier(new Soldier_entity(0))
        , render_thread(&Game::render, this)

    {
        map = Map::getInstance();
        map->load();
    }
    void render()
    {
        window.setActive(true);
    }
    void run()
    {
        window.setActive(false);
        render_thread.launch();
    }
};